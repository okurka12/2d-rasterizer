#include "bmp.h"
#include "utils.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define HEADER_SIZE 14
#define DIB_HEADER_SIZE 40

/* macro to be used in bmp_save, checks if `a` is equal to `b` 
  (the expected return value of a library function). 
  if it's not, call perror with `msg` and return 1. */
#define check(a, b, msg) if ((a) != (b)) { perror(msg); return 1; }

/* checks if system is little endian */
uint8_t is_little_endian() {
    unsigned int n = 1;
    return *(char *)&n == 1;
    /** it's worth mentioning here that making it compliant with big-endian
     * systems wouldn't be much of an issue but i dont want to mess
     * the code up with useless boilerplate */
}

/* sets appropriate values to 14-long byte array `header` */
void get_file_header(byte * header, uint16_t width, uint16_t height) {

    /* padding size (alignt to a multiple of 4 bytes) */
    uint32_t padding_size = 
        height * image_padding(width);
    
    /* file size */
    uint32_t filesize = HEADER_SIZE +
                        DIB_HEADER_SIZE + 
                        width * height * sizeof(color_t) +
                        padding_size;
    
    /* where the pixel array starts */
    uint32_t offset = HEADER_SIZE + DIB_HEADER_SIZE;
    
        header[0]  = 'B';                     // header field for BMP file
        header[1]  = 'M';
        header[2]  = ((byte *)&filesize)[0];  // file size
        header[3]  = ((byte *)&filesize)[1];
        header[4]  = ((byte *)&filesize)[2];
        header[5]  = ((byte *)&filesize)[3];
        header[6]  = 0;                       // reserved for whatever idk
        header[7]  = 0;
        header[8]  = 0;                       // reserved for whatever idk
        header[9]  = 0;
        header[10] = ((byte *)&offset)[0];    // offset
        header[11] = ((byte *)&offset)[1];    
        header[12] = ((byte *)&offset)[2];
        header[13] = ((byte *)&offset)[3];
    
}


/* sets appropriate values to 40-byte long byte array `header`
   (Windows BITMAPINFOHEADER) */
void get_dib_header(byte * header, uint32_t width, uint32_t height) {
    uint32_t dib_header_size = DIB_HEADER_SIZE;
    uint16_t color_planes_no = 1;
    uint16_t bits_per_pixel = 24;  // TrueColor
    uint32_t compression_method = 0;  // None
    uint32_t image_size = width * height * 3;  // size of the raw bitmap data
    uint32_t horizontal_resolution = 2808;
    uint32_t vertical_resolution = 2808;
    uint32_t color_pallete_no = 0;  // 0 to default of 2 to the n
    uint32_t important_color_no = 0;  // 0 - every color is important

    memcpy(header +  0, &dib_header_size, 4);
    memcpy(header +  4, &width, 4);
    memcpy(header +  8, &height, 4);
    memcpy(header + 12, &color_planes_no, 2);
    memcpy(header + 14, &bits_per_pixel, 2);
    memcpy(header + 16, &compression_method, 4);
    memcpy(header + 20, &image_size, 4);
    memcpy(header + 24, &horizontal_resolution, 4);
    memcpy(header + 28, &vertical_resolution, 4);
    memcpy(header + 32, &color_pallete_no, 4);
    memcpy(header + 36, &important_color_no, 4);

}


int bmp_save(char filename[], image_t *image) {

    static_assert(sizeof(color_t) == 3);
                
    if (!is_little_endian()) {
        fprintf(stderr, "this program cannot run on big-endian systems yet\n");
        return 1;
    }

    /* create file header and device independent bitmap header */
    byte file_header[HEADER_SIZE];
    byte dib_header[DIB_HEADER_SIZE];
    get_file_header(file_header, image->width, image->height);
    get_dib_header(dib_header, image->width, image->height);

    /* open file */
    FILE *f = fopen(filename, "w+");
    if (f == NULL) {
        fprintf(stderr, "could not create/rewrite %s\n", filename);
        return 1;
    }

    /* fwrite return value */
    size_t fwr = 0;

    /* write file header */
    logf("writing file header to %ld", ftell(f));
    fwr = fwrite(file_header, 1, HEADER_SIZE, f);
    check(fwr, HEADER_SIZE, "fwrite");

    /* write device independent bitmap header */
    logf("writing DIB header to %ld", ftell(f));
    fwr = fwrite(dib_header, 1, DIB_HEADER_SIZE, f);
    check(fwr, DIB_HEADER_SIZE, "fwrite");

    /* calculate the bitmap size */
    size_t bitmap_size = 
        image->width * image->height * sizeof(color_t) + // bitmap
        image->height * image_padding(image->width);  // padding
    
    /* write image data */
    logf("writing image array to %ld", ftell(f));
    fwr = fwrite(image->data, 1, bitmap_size, f);
    check(fwr, bitmap_size, "fwrite");

    /* close file */
    if (fclose(f) != 0) {
        perror("fclose");
        return 1;
    }

    return 0;   
}

/*

DIB-HEADER spec (https://en.wikipedia.org/wiki/BMP_file_format)

Offset-hex	Offset-dec Size-bytes Windows BITMAPINFOHEADER
0E          14         4          the size of this header, in bytes (40)
12          18         4          the bitmap width in pixels (signed integer)
16          22         4          the bitmap height in pixels (signed integer)
1A          26         2          the number of color planes (must be 1)
1C          28         2          the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.
1E          30         4          the compression method being used. See the next table for a list of possible values
22          34         4          the image size. This is the size of the raw bitmap data; a dummy 0 can be given for BI_RGB bitmaps.
26          38         4          the horizontal resolution of the image. (pixel per metre, signed integer)
2A          42         4          the vertical resolution of the image. (pixel per metre, signed integer)
2E          46         4          the number of colors in the color palette, or 0 to default to 2n
32          50         4          the number of important colors used, or 0 when every color is important; generally ignored

compression-method value
Value Identified-by  Compression-method               Comments
0  BI_RGB	         none	                          Most common
1  BI_RLE8	         RLE 8-bit/pixel	              Can be used only with 8-bit/pixel bitmaps
2  BI_RLE4	         RLE 4-bit/pixel	              Can be used only with 4-bit/pixel bitmaps
3  BI_BITFIELDS	     OS22XBITMAPHEADER: Huffman 1D	  BITMAPV2INFOHEADER: RGB bit field masks,
                                                                     BITMAPV3INFOHEADER+: RGBA
4  BI_JPEG	         OS22XBITMAPHEADER: RLE-24	      BITMAPV4INFOHEADER+: JPEG image for printing[14]
5  BI_PNG		     BITMAPV4INFOHEADER+:             PNG image for printing[14]
6  BI_ALPHABITFIELDS RGBA bit field masks	          only Windows CE 5.0 with .NET 4.0 or later
11 BI_CMYK	         none	                          only Windows Metafile CMYK
12 BI_CMYKRLE8	     RLE-8	                          only Windows Metafile CMYK
13 BI_CMYKRLE4	     RLE-4	                          only Windows Metafile CMYK

*/
