#include "bmp.h"
#include "utils.h"
#include <stdio.h>

int main() {

    timer_start(celkovy_cas);
    timer_start(generovani);

    /* this is only a test obviously main.c is not going to look like this */
    color_t red = {.red = 255};
    color_t yel = {.red = 255, .green = 255};
    // color_t cyan = {.green = 255, .blue = 255};
    
    image_t *image = bmp_image_ctor(3333, 2222);

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
            // image->data[image->width * y + x] = red;
        }
    }

    timer_end(generovani, "generovani obrazku");
    timer_start(ukladani);

    int return_value = bmp_save("obrazecek_prvni.bmp", image); 
    if (return_value) {
        fprintf(stderr, "nepodarilo se udelat obrazek, abort\n");
        return 1;
    } else {
        printf("obrazek uspesne vytvoren\n");
    }

    timer_end(ukladani, "ukladani");
    timer_end(celkovy_cas, "celkovy cas");

    return 0;
}