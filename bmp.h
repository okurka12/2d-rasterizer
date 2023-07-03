#include <stdint.h>

#ifndef _BMP_H
#define _BMP_H

/* struct for a single pixel */
typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} color_t;

/* struct for a pixel array and metadata */
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
int bmp_save(char filename[], image_t *image);

/* initializes image on heap, returns a pointer to it or NULL on failure */
image_t *bmp_image_ctor(uint32_t width, uint32_t height);

/* frees the image */
void bmp_image_dtor(image_t *image);

#endif  // ifndef _BMP_H
