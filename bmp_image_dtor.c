#include "bmp.h"
#include <stdlib.h>

void bmp_image_dtor(image_t *image) {
    free(image->data);
    free(image);
}
