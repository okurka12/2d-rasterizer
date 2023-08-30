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
 * comment this out if you want perfectly 1-pixel-wide circles 
 * this causes filled circles (circles of greater width) to not be filled 
 * perfectly
*/
#define CONTINUOUS_CIRCLE

/* returns true if a circle is in the image bounds */
static inline bool is_in_bounds(coord_t x, coord_t y, coord_t r, 
    image_t *img) {
        bool in_bounds = 1;
        if (x + r > img->width - 1) in_bounds = 0;
        if (y + r > img->height - 1) in_bounds = 0;
        if (x < r) in_bounds = 0;
        if (y < r) in_bounds = 0;
        return in_bounds;
    }

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
    pixel(x_mid + y, y_mid + x, img) = col;
    pixel(x_mid + y, y_mid - x, img) = col;
    pixel(x_mid - y, y_mid + x, img) = col;
    pixel(x_mid - y, y_mid - x, img) = col;
}

/* draws one pixel, bound checks, if it's out of bounds, draws nothing */
static inline void px_bound(coord_t x, coord_t y, color_t col, image_t *img) {
    if (x > img->width - 1) return;
    if (y > img->height - 1) return;
    pixel(x, y, img) = col;
}

/* draws pixels in all octants but performs a bound check for every write */
static inline void draw_8_pixels_bound(
    coord_t x,
    coord_t y,
    coord_t x_mid,
    coord_t y_mid,
    color_t col,
    image_t *img
) {
    px_bound(x_mid + x, y_mid + y, col, img);
    px_bound(x_mid + x, y_mid - y, col, img);
    px_bound(x_mid - x, y_mid + y, col, img);
    px_bound(x_mid - x, y_mid - y, col, img);
    px_bound(x_mid + y, y_mid + x, col, img);
    px_bound(x_mid + y, y_mid - x, col, img);
    px_bound(x_mid - y, y_mid + x, col, img);
    px_bound(x_mid - y, y_mid - x, col, img);
}

static inline void draw_blank_circle(point_t ct, coord_t r, color_t col, 
    bool in_bounds, image_t *img) {


    /* starting point for the iteration */
    coord_t x = 0;
    coord_t y = r;

    /* initial value of the predictor (see algorithm description) */
    int64_t p = 1 - (int64_t)r;

    /* iterate from 90 degrees down to 45 degrees, where x = y */
    while (x <= y) {

        /* draw pixels in all octants */
        if (in_bounds) {
            draw_8_pixels(x, y, ct.x, ct.y, col, img);

            #ifdef CONTINUOUS_CIRCLE
            draw_8_pixels(x, y - 1, ct.x, ct.y, col, img);
            #endif  // #ifdef CONTINUOUS_CIRCLE

        } else {
            draw_8_pixels_bound(x, y, ct.x, ct.y, col, img);

            #ifdef CONTINUOUS_CIRCLE
            draw_8_pixels_bound(x, y - 1, ct.x, ct.y, col, img);
            #endif  // #ifdef CONTINUOUS_CIRCLE
        }

        /* update the predictor */
        if (p <= 0) {
            p += 2 * x + 3;
        } else {
            p += 2 * x - 2 * (int64_t)y + 5;
            y--;
        }

        /* increment x */
        x++;
    }
}

void bmp_draw_circ(point_t ct, coord_t r, coord_t w, color_t col, 
    image_t *img) {

    /* is the entire circle in bounds? */
    signed int in_bounds = is_in_bounds(ct.x, ct.y, r, img);

    /* report if it's not */
    if (!in_bounds) {
        logf("Circle ct=(%u, %u) r=%u is out of bounds of img:%p", 
                 ct.x, ct.y, r, (void *)img);
    }

    /* is the width smaller than the radius? */
    coord_t r_less = r;
    if (w > r) {
        logf("Circle width %u larger than it's radius %u", w, r);
    }

    while (r_less > r - w) {
        draw_blank_circle(ct, r_less, col, in_bounds, img);
        r_less--;
    }

}
