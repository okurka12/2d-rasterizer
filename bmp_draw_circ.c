/**
 * A module for the library that draws a circle - either filled or not
 * It implements a midpoint algorithm and only computes that for the
 * first octant of the circle.
 * 
 *   ╲  ┃xx╱
 *    ╲ ┃x╱ 
 *  ━━━━╋━━━━
 *    ╱ ┃ ╲ 
 *   ╱  ┃  ╲
 * 
 *******************************************************************************
 * ALGORITHM DESCRIPTION
 *******************************************************************************
 * 
 * Assuming the center of the circle is in (0, 0) and the radius `r` the 
 * iteration begins in (0, r) and ends when the drawed line reaches 45 degrees
 * or, put simply, reaches the y = x line. In this region, the y coordinate
 * never decrements by more than a 1. Hence, we can iterate by x.
 * 
 * The equation for a circle is x^2 + y^2 - r^2 = 0. 
 * 
 * We also take advantage that the left side is less than zero if the point is
 * inside the circle and greater than zero if the point is outside the circle.
 * 
 * This way,knowing the coordinates of the midpoint (x + 1, y - 1/2), we can
 * easily calculate if the midpoint is inside or outside the circle by
 * substituting the midpoint's coordinates into the equation for a circle.
 * let's call this the _predictor_.
 * 
 * Additionally, we don't have to compute the expression for every increment
 * of x and y. Instead, let's solve by how much the predioctors's value
 * change when x increments by one (and optionally y decrements by one).
 * 
 * That goes:
 * p_{i + 1} = p_i + 2x + 3         if y was not decremented
 * p_{i + 1} = p_i + 2x - 2y + 5    if y was decremented
 * 
 * Once the predictor's value goes over zero, we decrement y.
 * 
 * The initial value of the predictor is p = 1 - r + 1/4, however, since
 * further on we will only be dealing with the predictor in terms of 
 * integer arithmetic, we can omit the 1/4.
 * 
*/

#include "bmp.h"
#include "utils.h"

/**
 * draws 8 pixels located at `x`, `y` symmetrically around the center of the 
 * circle `x_mid`, `y_mid` with color `col` to image `img`
 * 
 * NOTE: the `static` keyword limits the visibility to the translational unit
 * this is needed because otherwise the function gets inlined 
 * and thus eliminated completely, generating undefined reference error 
 * when linking against the library
*/
static inline void draw_8_pixels(
    coord_t x,
    coord_t y,
    coord_t x_mid,
    coord_t y_mid,
    color_t col,
    image_t *img
) {
    pixel(x_mid + x, y_mid + y, img) = col;
    pixel(x_mid + x, y_mid - y, img) = col;
    pixel(x_mid - x, y_mid + y, img) = col;
    pixel(x_mid - x, y_mid - y, img) = col;
    pixel(y_mid + y, x_mid + x, img) = col;
    pixel(y_mid + y, x_mid - x, img) = col;
    pixel(y_mid - y, x_mid + x, img) = col;
    pixel(y_mid - y, x_mid - x, img) = col;
}

void bmp_draw_circ(coord_t x, coord_t y, coord_t r, color_t col, image_t *img) {

    /* starting point for the iteration */
    coord_t xi = 0;
    coord_t yi = r;

    /* initial value of the predictor (see algorithm description) */
    int64_t p = 1 - (int64_t)r;

    /* iterate from 90 degrees down to 45 degrees, where x = y */
    while (xi < yi) {

        /* draw pixels in all octants */
        draw_8_pixels(xi, yi, x, y, col, img);

        /* update the predictor */
        if (p < 0) {
            p += 2 * xi + 3;
        } else {
            p += 2 * xi - 2 * (int64_t)yi + 5;
            yi--;
        }

        /* increment x */
        xi++;
    }

}