/**
 * lpt.c
 * 
 * Author: Chuck Valenza
 */

#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>

struct lp_cell {
    struct pixel* cent;
    double dist; // distance from center is the scaling factor
    double radius;
    long npixels;
    double x;
    double y;
    long r;
    long g;
    long b;
};

struct pixel {
    long x;
    long y;
    long r;
    long g;
    long b;
};

void lp_allocate_grid(struct lp_cell** *lp_grid, long width, long height);

void lp_init_grid(struct lp_cell** *lp_grid, double bounding_r, long out_w, long out_h);

void lp_transform(struct pixel** image_data, struct lp_cell** *lp_grid,
    long in_w, long in_h, long out_w, long out_h);
