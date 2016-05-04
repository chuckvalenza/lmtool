/**
 * lpt.c
 * 
 * Author: Chuck Valenza
 */

#include "lpt.h"
#include <math.h>

#define M_E 2.718281828459045235360287471352662497757247093699959574966

int in_circ(struct pixel* p, struct pixel* cent, double radius);
struct lp_cell* get_cell(struct pixel* p, struct lp_cell** *lp_grid);

/**
 * malloc a log-polar grid and initialize all values
 */
void lp_allocate_grid(struct lp_cell** *lp_grid, long width, long height)
{
    *lp_grid = malloc(sizeof(struct lp_cell*) * height);

    for (int y = 0; y < height; y++) {
        (*lp_grid)[y] = malloc(sizeof(struct lp_cell) * width);

        for (int x = 0; x < width; x++) {
            (*lp_grid)[y][x].cent = malloc(sizeof(struct pixel));
            (*lp_grid)[y][x].cent->x = 0;
            (*lp_grid)[y][x].cent->y = 0;
            (*lp_grid)[y][x].dist = 0;
            (*lp_grid)[y][x].radius = 0;
            (*lp_grid)[y][x].npixels = 0;
            (*lp_grid)[y][x].r = 0;
            (*lp_grid)[y][x].g = 0;
            (*lp_grid)[y][x].b = 0;
        }
    }
}

/**
 * Initialize the sizes of each cell in the log polar grid
 */
void lp_init_grid(struct lp_cell** *lp_grid, long out_w, long out_h)
{
    double radius = out_w;
    long prev_r;

    for (long row = 0; row < out_h; row++) {
        for (long col = out_w; col > 0; col--) {
            if ((long)radius == prev_r) {
                break;
            }

            prev_r = (long)radius;
            radius /= M_E;
            lp_grid[row][col - 1]->radius = radius;
        }
    }
}

/**
 * Perform the transformation by calculating the averages in each circle
 * and return the value in a linear grid: lp_grid
 */
void lp_transform(struct pixel** image_data, struct lp_cell** *lp_grid,
    long in_w, long in_h, long out_w, long out_h)
{
    struct pixel* cur_p = NULL;
    // center pixel of the image
    struct pixel* cent_p = malloc(sizeof(struct pixel));
    cent_p->x = in_w / 2;
    cent_p->y = in_h / 2;

    // only check if within largest circle
    for (int row = 0; row < in_h; row++) {
        for (int col = 0; col < in_w; col++) {
            cur_p = &(image_data[row][col]);

            // don't do anything unless in the radius of our overlayed circle
            if (in_circ(cur_p, cent_p, in_w / 2)) {
                // get the cell which the pixel belongs to
                struct lp_cell* cur_cell = get_cell(cur_p, lp_grid);

                // if the current pixel is in the circle approx of the current 
                // cell add it to the grey value of that current cell.
                if (in_circ(cur_p, cur_cell->cent, cur_cell->radius)) {
                    cur_cell->npixels++;
                    cur_cell->r += cur_p->r;
                    cur_cell->g += cur_p->g;
                    cur_cell->b += cur_p->b;
                }
            }
        }
    }
}

/**
 * Check if the pixel is within the range of our overlay image
 */
int in_circ(struct pixel* p, struct pixel* cent, double radius)
{
    // calculate distance from the center
    double x_sqr = pow(abs(p->x - cent->x), 2);
    double y_sqr = pow(abs(p->y - cent->y), 2);
    double dist = pow(x_sqr + y_sqr, 0.5);

    if (dist < radius) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * Return the pointer to the corresponding pointer of the current pixel
 */
struct lp_cell* get_cell(struct pixel* p, struct lp_cell** *lp_grid)
{
    // only implemented to avoid segfaults while testing
    // still needs implementation
    struct lp_cell* lpc = malloc(sizeof(struct lp_cell));

    lpc->cent = malloc(sizeof(struct pixel));
    lpc->cent->x = 0;
    lpc->cent->y = 0;
    lpc->dist = 0;
    lpc->npixels = 0;
    lpc->r = 0;
    lpc->g = 0;
    lpc->b = 0;

    return lpc;
}

