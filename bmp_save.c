#include "bmp.h"
#include <stdint.h>
#include <stdio.h>

#define HEADER_SIZE 14
#define DIB_HEADER_SIZE 12

typedef unsigned char byte;

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
    uint32_t filesize = 14 + 12 + width * height * 3;
    uint32_t offset = 14 + 12;  // where the pixel array starts
    
        header[0]  = 'B';                     // header field for BMP file
        header[1]  = 'M';
        header[2]  = ((byte *)&filesize)[0];
        header[3]  = ((byte *)&filesize)[1];
        header[4]  = ((byte *)&filesize)[2];
        header[5]  = ((byte *)&filesize)[3];
        header[6]  = 0;                       // reserved for whatever idk
        header[7]  = 0;
        header[8]  = 0;                       // reserved for whatever idk
        header[9]  = 0;
        header[10] = ((byte *)&offset)[0];
        header[11] = ((byte *)&offset)[1];    
        header[12] = ((byte *)&offset)[2];
        header[13] = ((byte *)&offset)[3];
    
}


/* sets appropriate values to 12-byte long byte array `header`
   (OS/2 1.x BITMAPCOREHEADER) */
void get_dib_header(byte * header, uint16_t width, uint16_t height) {
    uint32_t dib_header_size = 12;
    uint16_t color_planes_no = 1;
    uint16_t bits_per_pixel = 24;  // TrueColor

    header[0]  = ((byte *)&dib_header_size)[0];
    header[1]  = ((byte *)&dib_header_size)[1];
    header[2]  = ((byte *)&dib_header_size)[2];
    header[3]  = ((byte *)&dib_header_size)[3];
    header[4]  = ((byte *)&width)[0];
    header[5]  = ((byte *)&width)[1];
    header[6]  = ((byte *)&height)[0];
    header[7]  = ((byte *)&height)[1];
    header[8]  = ((byte *)&color_planes_no)[0];
    header[9]  = ((byte *)&color_planes_no)[1];
    header[10] = ((byte *)&bits_per_pixel)[0];
    header[11] = ((byte *)&bits_per_pixel)[1];
}


int bmp_save(char filename[], color_t data[], uint16_t width, 
               uint16_t height) {
                
    if (!is_little_endian()) {
        fprintf(stderr, "this program cannot run on big-endian systems yet\n");
        return 1;
    }

    /* create file header and device independent bitmap header */
    byte file_header[HEADER_SIZE];
    byte dib_header[DIB_HEADER_SIZE];
    get_file_header(file_header, width, height);
    get_dib_header(dib_header, width, height);

    /* open file */
    FILE *f = fopen(filename, "w+");
    if (f == NULL) {
        fprintf(stderr, "could not create/rewrite %s\n", filename);
        return 1;
    }

    /* write file header */
    for (uint8_t i = 0; i < HEADER_SIZE; i++) {
        fputc(file_header[i], f);
    }

    /* write device independent bitmap header */
    for (uint8_t i = 0; i < DIB_HEADER_SIZE; i++) {
        fputc(dib_header[i], f);
    }

    /* write image data */
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            fputc(data[width * y + x].blue, f);
            fputc(data[width * y + x].green, f);
            fputc(data[width * y + x].red, f);
        }
    }

    /* close file */
    fclose(f);

    return 0;   
}
