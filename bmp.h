#include <stdint.h>

#ifndef _BMP_H
#define _BMP_H

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} color_t;

/** 
 * creates specified bmp file with provided image data 
 * (single-dimensional array of color_t)
 * returns 0 on success, 1 otherwise 
 */
int create_bmp(
                    char filename[], 
                    color_t data[], 
                    uint16_t width, 
                    uint16_t height
);



#endif  // ifndef _BMP_H
