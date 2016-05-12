/**
 * lpt.c
 * 
 * Author: Chuck Valenza
 */

#include <stdio.h>
#include <stdlib.h>

struct point {
    double x;
    double y;
};

struct lp_grid {
    struct lp_cell** grid;
    long width;
    long height;
};

struct lp_cell {
    struct point loc;
    double dist;
    double radius;
    long npixels;
    double r;
    double g;
    double b;
};

struct pixel {
    struct point loc;
    double r;
    double g;
    double b;
};

void lp_create_grid(struct lp_grid* g, long output_width, long output_height);

void lp_allocate_grid(struct lp_grid* g);

void lp_init_grid(struct lp_grid* g, double ovl_rad);

void lp_transform(struct lp_grid* grid, struct pixel** image_data,
    long in_w, long in_h);
