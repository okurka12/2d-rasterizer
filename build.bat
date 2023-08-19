:: build script to build all components of 2d-rasterizer on windows
:: prerequisites: Visual Studio Build Tools (Desktop development with C++)
:: this script is to be run in the developer command prompt

:: works with
:: Visual Studio build Tools 2022
:: Microsoft (R) C/C++ Optimizing Compiler Version 19.35.32215 for x86
:: Microsoft (R) Incremental Linker Version 14.35.32215.0


@set CC=cl
@set CFLAGS=/Wall /EHsc /MD
@set LIB_FILENAME=libbmap.lib
@set DLL_FILENAME=libbmap.dll

:: modules
@set modules=bmp_draw_rect bmp_image_ctor bmp_image_dtor bmp_save

:: modules but with .obj extension
@setlocal EnableDelayedExpansion
@set objmodules=
@ for %%m in (%modules%) do @set objmodules=!objmodules! %%m.obj

:: modules but as export linker directives (`/EXPORT module`)
@setlocal EnableDelayedExpansion
@set expmodules=
@ for %%m in (%modules%) do @set expmodules=!expmodules! /EXPORT:%%m

:: compile modules into object files
@ for %%m in (%modules%) do %CC% %CFLAGS% /c %%m.c

:: link modules into a library
link /DLL /out:%DLL_FILENAME% %expmodules% %objmodules%
:: TODO: implib a takovy picoviny

:: compile example
%CC% %CFLAGS% /Fe:example.exe example.c %LIB_FILENAME%
