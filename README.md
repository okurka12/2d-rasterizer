# rasterizer

Limits:
- Picture can be 65536 pixels tall and wide, but
- Width times height must not exceed 1431655756 px^2

This program outputs BMP files with following parameters:
- `OS/2 1.x BITMAPCOREHEADER` header
- TrueType colors (24 bits per pixel)
- uncompressed (obviously, it's BMP)
