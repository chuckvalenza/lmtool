/**
 * lpt.c
 * 
 * Author: Chuck Valenza
 */

#include "lpt.h"
#include <math.h>

/**
 * malloc a log-polar grid and initialize all values
 */
struct lp_cell** allocate_lparray(long width, long height)
{
    struct lp_cell** lparr = malloc(sizeof(struct lp_cell*) * width);

    for (int i = 0; i < width; i++) {
        lparr[i] = malloc(sizeof(struct lp_cell) * height);
        lparr[i]->cent = malloc(sizeof(struct pixel));
        lparr[i]->cent->x = 0;
        lparr[i]->cent->y = 0;
        lparr[i]->dist = 0;
        lparr[i]->npixels = 0;
        lparr[i]->r = 0;
        lparr[i]->g = 0;
        lparr[i]->b = 0;
    }

    return lparr;
}

/**
 * Perform the transformation by calculating the averages in each circle
 * and return the value in a linear grid: lp_arr
 */
void lp_transform(unsigned char* image_data, struct lp_cell* **lp_arr,
    unsigned long byte_count, long in_w, long in_h, long out_w, long out_h)
{
    struct pixel* cur_p = NULL;
    // center pixel of the image
    struct pixel* cent_p = malloc(sizeof(struct pixel));
    cent_p->x = in_w / 2;
    cent_p->y = in_h / 2;

    // only check if within largest circle
    for (int row = 0; row < in_h; row++) {
        for (int col = 0; col < in_w; col++) {
            cur_p = malloc(sizeof(struct pixel));
            cur_p->x = col;
            cur_p->y = row;

            // don't do anything unless in the radius of our overlayed circle
            if (in_circ(cur_p, cent_p, in_w / 2)) {
                // get the cell which the pixel belongs to
                struct lp_cell* cur_cell = get_cell(cur_p);

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
struct lp_cell* get_cell(struct pixel* p)
{
    struct lp_cell* lpc;

    return lpc;
}

