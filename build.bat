:: build script to build all components of 2d-rasterizer on windows
:: prerequisites: Visual Studio Build Tools (Desktop development with C++)
:: this script is to be run in the developer command prompt

:: usage
:: .\build.bat                 builds dynamic library (+ example.exe)
:: .\build.bat static          builds static library (+ example.exe)
:: .\build.bat clean           removes all binaries (like `make clean`)

:: works with
:: Visual Studio build Tools 2022
:: Microsoft (R) C/C++ Optimizing Compiler Version 19.35.32215 for x86
:: Microsoft (R) Incremental Linker Version 14.35.32215.0

:: See https://stackoverflow.com/a/51861571/15604091 to know if a .lib file
:: is a static library or an import library for a dynamic library
:: beware: a .lib file can be combination of both

:: the C compiler
@set CC=cl

:: c compiler flags: uncomment one
::@set CFLAGS=/Wall /EHsc /MD /fsanitize=address /Zi
::@set CFLAGS=/Wall /EHsc /MD /DNDEBUG
@set CFLAGS=/Wall /EHsc /MD

:: set this to /DEBUG if you want the DLL to contain debug info
@set LDFLAGS=/DEBUG

:: filenames
@set LIB_FILENAME=libbmap.lib
@set DLL_FILENAME=libbmap.dll

:: modules
@set modules=bmp_draw_rect bmp_image_ctor bmp_image_dtor bmp_save bmp_draw_circ
@set modules=%modules% bmp_draw_line

:: .\build.bat clean
@ if "%1" equ "clean" (
    @ goto :do_clean
)
goto :dont_clean
:do_clean
    del *.o *.bin *.so *.a *.bmp *.obj *.exe *.dll *.exp *.lib *.pdb *.ilk
    @exit /b
:dont_clean

:: create variable `modules` but with .obj extension
@setlocal EnableDelayedExpansion
@set objmodules=
@ for %%m in (%modules%) do @set objmodules=!objmodules! %%m.obj

:: create variable `modules` but as linker export directives (`/EXPORT module`)
@setlocal EnableDelayedExpansion
@set expmodules=
@ for %%m in (%modules%) do @set expmodules=!expmodules! /EXPORT:%%m

:: compile modules into object files
@ for %%m in (%modules%) do %CC% %CFLAGS% /c %%m.c

:: .\build.bat static
@ if "%1" equ "static" ( call :build_static ) else ( call :build_dynamic )

:: here, I decided to put the commands into separate functions, so that
:: the if-statement itself is not echoed, 
:: but the command that creates the library is called

:: FUNCTIONS
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: this is to skip direct execution of this block (so that the 
:: functions are not executed directly, only by being called)
@ goto :functions_end

:build_static

    :: create a static library (.lib)
    lib /out:%LIB_FILENAME% %objmodules%

    @ exit /b 0

:build_dynamic

    :: create a dynamic library (.dll)
    link /DLL %LDFLAGS% /out:%DLL_FILENAME% %expmodules% %objmodules%

    @ exit /b 0

:run_example
    .\example.exe
    start example.bmp
    @ exit /b 0

:: end of the function block, from here, the code is again executed directly
:functions_end
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: compile example.c and link 
%CC% %CFLAGS% /Fe:example.exe example.c %LIB_FILENAME%

@ if "%1" equ "run" ( call :run_example )
