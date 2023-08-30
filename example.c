#include "bmp.h"
#include "utils.h"
#include <stdio.h>

#define WIDTH 1000
#define HEIGHT 1000

const color_t red = {.red = 255};
const color_t grn = {.green = 255};
const color_t blu = {.blue = 255};
const color_t yel = {.red = 255, .green = 255};
const color_t cyan = {.green = 255, .blue = 255};
const color_t white = { .red=255, .green = 255, .blue = 255};

/* generates image */
void generate_image(image_t *img) {

    coord_t circle_width = 10;
    
    /* red rectangle under the big yellow circle*/
    bmp_draw_rect((point_t){ .x = 175, .y = 175}, 
                  (point_t){ .x = 350, .y = 350}, red, img);

    /* big yellow circle in the middle */
    bmp_draw_circ((point_t){ .x = 500, .y = 500}, 400, circle_width, yel, img);

    /* small green circle out of bound in the upper-right */
    bmp_draw_circ((point_t){ .x = 990, .y = 990}, 50, circle_width, grn, img);

    /* filled white circle in the upper-left */
    bmp_draw_circ((point_t){ .x = 150, .y = 850}, 80, 80, white, img);
    
    /* small blue rectangle in the lower-left */
    bmp_draw_rect((point_t){.x = 10, .y = 10}, 
                  (point_t){.x = 50, .y = 50}, blu, img);
}

int main(void) {

    /* timer thingies */
    timer_start(time_total);
    timer_start(time_generation);
    
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
    timer_end(time_generation, "image generation");
    timer_start(time_saving);

    /* save image into file */
    int return_value = bmp_save("example.bmp", image); 
    
    /* free image */
    bmp_image_dtor(image);

    /* check if saving into file was successful */
    if (return_value) {
        fprintf(stderr, "Image couldn't be saved, abort\n");
        return 1;
    } else {
        printf("Image successfully generated and saved.\n");
    }

    /* timer thingies */
    timer_end(time_saving, "image saving");
    timer_end(time_total, "total CPU time");

    return 0;
}