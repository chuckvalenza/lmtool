/**
 * lpt.c
 * 
 * Author: Chuck Valenza
 */

#include <stdio.h>
#include <stdlib.h>

struct lp_cell {
    long npixels;
    double dist; // distance from center is the scaling factor
    long r;
    long g;
    long b;
};

struct pixel {
    long x;
    long y;
};

struct lp_cell** allocate_lparray(long width, long height);

void lp_transform(unsigned char* image_data, struct lp_cell* **lp_arr,
    unsigned long byte_count, long in_w, long in_h, long out_w, long out_h);

int in_overlay_range(struct pixel* p, long w, long h);
