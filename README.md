# 2d rasterizer
This is one of my first successful C projects. A C library that construts an image, lets you access pixels individually or use functions to draw shapes, then save it as a `.bmp` file.

## Story behind 2d-rasterizer
Initially, a long time ago, I made module that would take an array of pixels and save it as a `.bmp` file. 

Then, I restructured it to be a library. Both a static library (`.a`) and a dynamic library (`.so`/`.dll`).

Then, I made the thing more useful, better, faster. (See **features**)

After I completed `bmp_draw_rect`, a function that can actually draw to the image, I wrote a python wrapper for the library.

## Features

A `.bmp` picture file has the rows padded to 4 bytes. I made it so that the picture is row-padded from the beginning, that is, the padding is present in the pixel array in memory when the picture is still being generated. 

How is the pixel's position in the array calculated?
- `pixel(x, y, img)` macro, that returns an l-value of a pixel struct at `x`, `y` in the  `img`'s pixel array (`img` being an image struct)

Whats the benefit of this?
- The image is written to the `.bmp` file using only three `fwrite` calls. (File header, DIB header, the data)
- This makes it fast because there is very little I/O overhead

## Compilation/build process
### On GNU/Linux systems:
I developed the library using `gcc 13.1.0`, but really any version should work. Also I used `GNU Make 4.3`.

- `make` to compile everything, static and dynamic versions of the library and `example.bin`

- `make libbmap.a` to compile the static library (`.a`)

- `make libbmap.so` to compile the dynamic library (`.so`)

- `make clean` to remove all binaries (so you can re-start the build process froms cratch)

Know that `make run` is for development only - I use a very specific environment that might not work for you

### On Windows
While you could use any compiler to build this library (perhaps MinGW), I chose MSVC. Therefore, to use the provided build script, you need to have `Microsoft Visual Studio Build Tools` (when installing, select `Desktop development with C++` workload).

- `build.bat` to build the dynamic library (`.dll`) and `example.exe`
- `build.bat static` to build the static library (`.lib`) and `example.exe`
- `build.bat clean` to remove all binaries (sou you can re-build from scratch)

Note: A `.lib` file is also created alongside the `.dll` dynamic library. However, this file is not a static library, rather a so-called *import library*. This is what you then link your program against (you still need the `.dll`, the code is in it).

## Example image
The `example.c` file is compiled to `example.bin` (perhaps `example.exe`). When `example.bin` is run, it creates `example.bmp`, an example image.

## Future goals
In the near future, I want to:
- Implement more drawing functions
- ✅ Work on the python wrapper (add missing null checks, make it more pythonic, etc.)
- ✅ Automate + document the build process on OS Windows

## Implementing new functions to the library
To add a new function, one must:
- implement it in a spearate `.c` file
- write a recipe for it in `Makefile`
  - that will most probably go like this:
  ```
  # compile my_module
  my_module.o: my_module.c
  	$(CC) $(CFLAGS) -fPIC -c $<
  ```
- in `Makefile`, add it to the `BMP_LIB_MODULES` variable
- in `build.bat`, add it to the `modules` variable

That way, upon finishing implementation, the function is exported to the library.