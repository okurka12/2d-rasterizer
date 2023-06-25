#include "bmp.h"
#include <stdio.h>

int main() {

    /* this is only a test obviously main.c is not going to look like this */
    color_t red = {.red = 255};
    color_t yel = {.red = 255, .green = 255};
    // color_t cyan = {.green = 255, .blue = 255};
    
    image_t *image = bmp_image_ctor(100, 50);

    if (image == NULL) {
        fprintf(stderr, "couldnt allocate or sth\n");
        return 1;
    }

        for (unsigned int y = 0; y < image->height; y++) {
        for (unsigned int x = 0; x < image->width; x++) {
            if (x < image->width / 2 || y < image->height / 2) {
                image->data[image->width * y + x] = yel;
            }
            else {
                image->data[image->width * y + x] = red;
            }
        }
    }

    int return_value = bmp_save("obrazecek_prvni.bmp", image); 
    if (return_value) {
        fprintf(stderr, "nepodarilo se udelat obrazek, abort\n");
        return 1;
    } else {
        printf("obrazek uspesne vytvoren\n");
    }

    return 0;
}