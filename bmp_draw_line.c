/**
 * bmp_draw_line
 * implements digital differential analyzer algorithm
*/
#include "bmp.h"
#include "utils.h"



/* Shift value by how many bits? */
#define FRAC_BITS 8

void bmp_draw_line(point_t a, point_t b, color_t col, coord_t t, image_t *img) {

    logf("%s called", __func__);

    /* TODO: */
    (void)t;

    int64_t dx = (int64_t)b.x - a.x;
    int64_t dy = (int64_t)b.y - a.y;
    logf("dx=%lld dy=%lld", dx, dy);

    point_t start;
    point_t end;

    if (dy > dx) {

        /* iterate by y */
        /*--------------------------------------------------------------------*/
        

        /* go from a to b or from b to a? */
        if (b.y < a.y) {
            start = b;
            end = a;
        } else {
            start = a;
            end = b;
        }

        /* differential (upscaled) */
        int64_t k = (dx << FRAC_BITS) / dy;

        /* initial x coordinate (upscaled) */
        coord_t x = start.x << FRAC_BITS;

        /* iterate */
        for (coord_t y = start.y; y < start.y + dy; y++) {

            /* put the pixel (first, downscale the x coordinate) */
            pixel(x >> FRAC_BITS, y, img) = col;

            /* add the differential */
            x += k;
        }


    } else {

        /* iterate by x */
        /*--------------------------------------------------------------------*/
        
        /* go from a to b or from b to a? */
        if (b.x < a.x) {
            start = b;
            end = a;
        } else {
            start = a;
            end = b;
        }

        /* differential */
        int64_t k = (dy << FRAC_BITS) / dx;

        /* initial y coordinate */
        coord_t y = start.y << FRAC_BITS;
        for (coord_t x = start.x; x < start.x + dx; x++) {
            pixel(x, y >> FRAC_BITS, img) = col;
            y += k;
        }

    }


}