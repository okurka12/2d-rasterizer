#include "bmp.h"
#include "utils.h"



/* Shift value by how many bits? */
#define FRAC_BITS 8

void bmp_draw_line(point_t a, point_t b, color_t col, coord_t t, image_t *img) {
    (void)a;
    (void)b;
    (void)col;
    (void)t;
    (void)img;

    int64_t dx = (int64_t)b.x - a.x;
    int64_t dy = (int64_t)b.y - a.y;
    logf("dx=%lld dy=%lld", dx, dy);

    point_t start;
    point_t end;

    if (dy > dx) {

        /* iterate by y */
        

        /* go from a to b or from b to a? */
        if (b.y < a.y) {
            start = b;
            end = a;
        } else {
            start = a;
            end = b;
        }

        /* increment or decrement x? */
        // signed int x_step = start.x < end.x ? -1 : 1

        /* differential */
        int64_t k = (dx << FRAC_BITS) / dy;

        coord_t x = start.x << FRAC_BITS;
        for (coord_t y = start.y; y < start.y + dy; y++) {
            logf("putting %u, %u", x >> FRAC_BITS, y);
            pixel(x >> FRAC_BITS, y, img) = col;
            x += k;
        }


    }
}