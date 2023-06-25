#include "bmp.h"
#include <stdlib.h>

image_t *bmp_image_ctor(uint32_t width, uint32_t height) {
    
    /* allocate room for the struct itself */
    image_t *output = malloc(sizeof(image_t));
    
    /* null check */
    if (output == NULL) {
        return NULL;
    }

    /* initialize struct */
    output->width = width;
    output->height = height;
    output->data = malloc(width*height*sizeof(color_t));  // allocate

    /* null check */
    if (output->data == NULL) {
        free(output);
        return NULL;
    }
    
    return output;
}
