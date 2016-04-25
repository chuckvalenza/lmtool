/**
 * lpt.c
 * 
 * Author: Chuck Valenza
 */

#include "lpt.h"
#include <math.h>

/**
 * malloc a log-polar grid
 */
struct lp_cell** allocate_lparray(long width, long height)
{
    struct lp_cell** lparr = malloc(sizeof(struct lp_cell*) * width);

    for (int i = 0; i < width; i++) {
        lparr[i] = malloc(sizeof(struct lp_cell) * height);
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
    // center pixel of the image
    struct pixel* cent_p = malloc(sizeof(struct pixel));
    cent_p->x = in_w / 2;
    cent_p->y = in_h / 2;

    // only check if within largest circle
    for (int row = 0; row < in_w; row++) {
        for (int col = 0; col < in_h; col++) {
            struct pixel* cur_p = malloc(sizeof(struct pixel));
            cur_p->x = col;
            cur_p->y = row;

            // don't do anything unless in the radius of our overlayed circle
            if (in_circ(cur_p, cent_p, in_w / 2)) {
                fprintf(stderr, "computing for pixel in overlay\n");
            } else {
                fprintf(stderr, "discarding pixel out of overlay\n");
            }
        }
    }
}

/**
 * check if the pixel is within the range of our overlay image
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

