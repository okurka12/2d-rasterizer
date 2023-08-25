#include "bmp.h"
#include "utils.h"
#include <stdio.h>

#define WIDTH 1000
#define HEIGHT 1000

const color_t red = {.red = 255};
const color_t yel = {.red = 255, .green = 255};
const color_t cyan = {.green = 255, .blue = 255};
const color_t white = { .red=255, .green = 255, .blue = 255};

/* generates image */
void generate_image(image_t *img) {
    
    bmp_draw_circ(500, 500, 400, white, img);
    
    bmp_draw_rect(
        (point_t){.x = 10, .y = 10}, 
        (point_t){.x = 50, .y = 50}, 
        yel, 
        img
    );
}

int main(void) {

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
    int return_value = bmp_save("example.bmp", image); 
    
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