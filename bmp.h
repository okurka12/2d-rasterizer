#include <stdint.h>

#ifndef _BMP_H
#define _BMP_H

/* align to a multiple of how many bytes */
#define ROW_PADDING 4

/* byte */
typedef unsigned char byte;

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

    /* the bitmap itself (row-padded) */
    byte *data;

} image_t;

/* calculates appropriate padding (align `n` to `align`) */
#define calc_pad_siz(n, align) \
    ((n) % (align) == 0 ? 0 : (align) - ((n) % (align)))

/* calculates how many bytes are necessary to pad a color_t row of `width` */
#define image_padding(width) \
    calc_pad_siz((width) * sizeof(color_t), ROW_PADDING)

/* a row width (`image_t img`) in bytes including the padding */
#define row_width(img) \
    (img->width * sizeof(color_t) + image_padding(img->width))

/* returns a color_t l-value of a pixel at `x`, `y` in `img` (image_t) */
#define pixel(x, y, img) \
    *((color_t *)(img->data + (y) * row_width(img) + (x) * sizeof(color_t)))

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
