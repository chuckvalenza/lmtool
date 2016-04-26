/**
 * lpt.c
 * 
 * Author: Chuck Valenza
 */

#include <stdio.h>
#include <stdlib.h>

struct lp_cell {
    struct pixel* cent;
    double dist; // distance from center is the scaling factor
    double radius;
    long npixels;
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

struct lp_cell** lp_allocate_grid(long width, long height);

void lp_transform(unsigned char* image_data, struct lp_cell* **lp_arr,
    unsigned long byte_count, long in_w, long in_h, long out_w, long out_h);

