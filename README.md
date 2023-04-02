# rasterizer

I wrote a C module that takes a bitmap array and produces `.bmp` file out of it. It's pretty neat I think, given that this was one of my first (successful) C projects. The `main` module is not even a main module, it's just something that demonstrates the use of `bmp` module. 

Limits:
- Picture can be 65536 pixels tall and wide, but
- Width times height must not exceed 1431655756 px^2

This program outputs BMP files with following parameters:
- `OS/2 1.x BITMAPCOREHEADER` header
- TrueType colors (24 bits per pixel)
- uncompressed (obviously, it's BMP)

I developed it on Windows 11 using `gcc (MinGW.org GCC-6.3.0-1) 6.3.0`. But it really doesn't matter which compiler on what system you use, nothing platform/compiler-dependent was used.
