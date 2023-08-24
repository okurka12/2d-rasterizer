#include "bmp.h"
#include <stdlib.h>

image_t *bmp_image_ctor(coord_t width, coord_t height) {
    
    /* allocate room for the struct itself */
    image_t *output = malloc(sizeof(image_t));
    
    /* null check */
    if (output == NULL) {
        return NULL;
    }

    /* initialize the image struct */
    output->width = width;
    output->height = height;

    /* allocate room for the bitmap */
    output->data = calloc(
        1, width*height*sizeof(color_t) + height*image_padding(width)
    );

    /**
     * why calloc?
     * that's so that the padding bytes are always zero - they're never
     * directly accessed
    */

    /* null check */
    if (output->data == NULL) {
        free(output);
        return NULL;
    }
    
    return output;
}
