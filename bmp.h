#include <stdint.h>

#ifndef _BMP_H
#define _BMP_H

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} color_t;

typedef struct {

    /* image width */
    uint32_t width;

    /* image height */
    uint32_t height;

    /* array of pixels (bitmap) */
    color_t *data;

} image_t;

/** 
 * creates specified bmp file with provided image struct
 * returns 0 on success, 1 otherwise 
 */
int create_bmp(char filename[], image_t image);



#endif  // ifndef _BMP_H
