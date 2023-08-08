#include "bmp.h"

void bmp_draw_rect(point_t a, point_t b, color_t col, image_t *image) {

    /* variables for bounds */
    coord_t start_x = a.x;
    coord_t start_y = a.y;
    coord_t end_x = b.x;
    coord_t end_y = b.y;
    
    /* bound checks */
    if (start_x > image->width) start_x = image->width;
    if (start_y > image->height) start_y = image->height;
    if (end_x > image->width) end_x = image->width;
    if (end_y > image->height) end_y = image->height;
    
    /* iterating */
    for (coord_t x = start_x; x < end_x; x++) {
        for (coord_t y = start_y; y < end_y; y++) {
            pixel(x, y, image) = col;
        }
    }
    
}