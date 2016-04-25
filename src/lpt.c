/**
 * lpt.c
 * 
 * Author: Chuck Valenza
 */

#include "lpt.h"

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
    // only check if within largest circle
    // create largest circle by 
    for (int row = 0; row < in_w; row++) {
        for (int col = 0; col < in_h; col++) {
            struct pixel* cur = malloc(sizeof(struct pixel));
            cur->x = col;
            cur->y = row;

            // don't do anything unless in the radius of our overlayed circle
            if (in_overlay_range(cur, in_w, in_h)) {
                fprintf(stderr, "computing for pixel in overlay");
            } else {
                fprintf(stderr, "discarding pixel out of overlay");
            }
        }
    }
}

/**
 * check if the pixel is within the range of our overlay image
 */
int in_overlay_range(struct pixel* p, long w, long h)
{
    long radius = w / 2;
    struct pixel* cent = malloc(sizeof(struct pixel));

    cent->x = w / 2;
    cent->y = h / 2;

    // sqrt(x^2 + y^2)
    long dist = pow((pow(x, 2) + pow(x, 2)), 0.5);
    if (dist < radius) {
        return 0;
    } else {
        return 1;
    }
}

