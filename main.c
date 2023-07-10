#include "bmp.h"
#include "utils.h"
#include <stdio.h>

#define WIDTH 400
#define HEIGHT 300

const color_t red = {.red = 255};
const color_t yel = {.red = 255, .green = 255};
const color_t cyan = {.green = 255, .blue = 255};

/* fills a square of `n` by `n` at `x`, `y` with color `col`*/
void fill_sq(uint32_t x, uint32_t y, uint32_t n, color_t col, image_t *img) {
    log("fill_sq zavolan");
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < n; j++) {
            logf("nastavuji pixel (%u, %u) na col", x + j, y + i);
            pixel(x + j, y + i, img) = col;
        }
    }
}

/* generates image */
void generate_image(image_t *img) {
    fill_sq(10, 10, 10, red, img);
    fill_sq(90, 5, 10, red, img);
    fill_sq(300, 200, 10, red, img);
}

int main() {

    /* timer thingies */
    timer_start(celkovy_cas);
    timer_start(generovani);
    
    /* initialize empty image */
    image_t *image = bmp_image_ctor(WIDTH, HEIGHT);

    /* null check */
    if (image == NULL) {
        fprintf(stderr, "couldnt allocate or sth\n");
        return 1;
    }

    /* generating of the picture itself */
    generate_image(image);

    /* timer thingies */
    timer_end(generovani, "generovani obrazku");
    timer_start(ukladani);

    /* save image into file */
    int return_value = bmp_save("obrazecek_prvni.bmp", image); 
    
    /* free image */
    bmp_image_dtor(image);

    /* check if saving into file was successful */
    if (return_value) {
        fprintf(stderr, "nepodarilo se udelat obrazek, abort\n");
        return 1;
    } else {
        printf("obrazek uspesne vytvoren\n");
    }

    /* timer thingies */
    timer_end(ukladani, "ukladani");
    timer_end(celkovy_cas, "celkovy cas");

    return 0;
}