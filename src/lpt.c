/**
 * lpt.c
 * 
 * Author: Chuck Valenza
 */

#include "lpt.h"
#include <math.h>

#define M_E 2.718281828459045235360287471352662497757247093699959574966
#define M_PI 3.141592653589793238462643383279502884197169399375105820974

double radians = M_PI / 180.0;


short in_circ(struct point p, struct point cent, double radius);
struct lp_cell* get_cell(struct lp_grid* g, struct pixel* p);

/**
 * Initialize the values in a grid object
 */
void lp_create_grid(struct lp_grid* g, long output_width, long output_height)
{
    g->width = output_width;
    g->height = output_height;

    lp_allocate_grid(g);
}

/**
 * malloc a log-polar grid and initialize all values so fovea is at the left
 * 
 * Takes one parameter: output lp_grid
 */
void lp_allocate_grid(struct lp_grid* g)
{
    g->grid = malloc(sizeof(struct lp_cell*) * g->width);

    for (long x = 0; x < g->width; x++) {
        g->grid[x] = malloc(sizeof(struct lp_cell) * g->height);

        for (long y = 0; y < g->height; y++) {
            g->grid[x][y].loc.x = 0;
            g->grid[x][y].loc.y = 0;
            g->grid[x][y].dist = 0;
            g->grid[x][y].radius = 0;
            g->grid[x][y].npixels = 0;
            g->grid[x][y].r = 0;
            g->grid[x][y].g = 0;
            g->grid[x][y].b = 0;
        }
    }
}

/**
 * Initialize the sizes of each cell in the log polar grid corresponding to the
 * input image; creates the overlay.
 * 
 * Takes two arguments: ouput lp_grid, overlay's radius
 * 
 * There are two possible scenarios which we must look out for when creating
 * the grid: 1. the user has requested too many cells and the radius of the
 * cell becomes smaller than the size of a pixel, and 2. the distance from 
 * the center of the image approaches zero.
 */
void lp_init_grid(struct lp_grid* g, double ovl_rad)
{
    double dist;
    double prev_d;

    double theta;
    double d_theta = 360.0 / g->width;

    for (long x = g->width - 1; x >= 0; x--) {
        dist = ovl_rad;

        for (long y = 0; y < g->height; y++) {
            struct lp_cell* cur = &(g->grid[x][y]);

            // calculate radius of cell; trunc to zero if it is too small
            cur->radius = dist / (2 * M_E);

            if (cur->radius * 2 < 1) {
                cur->radius = 0;
            }

            // calculate distance from center of the overlay
            prev_d = dist;
            dist -= cur->radius * 2;
            cur->dist = dist + ((prev_d - dist) / 2);

            // calculate x and y coordinates of the center of each cell
            theta = y * d_theta * radians;

            if (theta < 0.5 * M_PI) {
                cur->loc.x = ovl_rad + cur->dist * cos(theta);
                cur->loc.y = cur->dist * sin(theta);
            } else if (theta < M_PI) {
                cur->loc.x = cur->dist * cos(theta);
                cur->loc.y = cur->dist * sin(theta);
            } else if (theta < 1.5 * M_PI) {
                cur->loc.x = cur->dist * cos(theta);
                cur->loc.y = ovl_rad + cur->dist * sin(theta);
            } else {
                cur->loc.x = ovl_rad + cur->dist * cos(theta);
                cur->loc.y = ovl_rad + cur->dist * sin(theta);
            }
        }
    }
}

/**
 * Perform the transformation by calculating the averages in each circle
 * and return the value in a linear grid: lp_grid
 * 
 * Takes four arguments: ouput lp_grid, input image pixel data,
 *     input image width, input image height
 */
void lp_transform(struct lp_grid* g, struct pixel** image_data,
    long in_w, long in_h)
{
    struct point img_cent;
    img_cent.x = in_w / 2.0;
    img_cent.x = in_w / 2.0;

    struct pixel* cur_pix;
    struct lp_cell* cur_cell = NULL;

    for (long x = 0; x < in_w; x++) {
        for (long y = 0; y < in_h; y++) {
            cur_pix = &(image_data[x][y]);

            // only proceed if the pixel is within the overlay
            if (in_circ(cur_pix->loc, img_cent, img_cent.x)) {
                // get the cell which the pixel belongs to
                cur_cell = get_cell(g, cur_pix);

                // if the current pixel is in the circle approx of the current 
                // cell add it to the grey value of that current cell.
                if (in_circ(cur_pix->loc, cur_cell->loc, cur_cell->radius)) {


                    cur_cell->npixels++;
                    cur_cell->r += cur_pix->r;
                    cur_cell->g += cur_pix->g;
                    cur_cell->b += cur_pix->b;
                }
            }
        }
    }
}

/**
 * Check if the pixel is within the range of our overlay image
 */
short in_circ(struct point p, struct point cent, double radius)
{
    double dist_x = pow(abs(p.x - cent.x), 2);
    double dist_y = pow(abs(p.y - cent.y), 2);

    if (pow(dist_x + dist_y, 0.5) < radius) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * Return the pointer to the corresponding pointer of the current pixel
 * 
 * This needs to use a hash map to get the current cell. Will be implemented
 * later, but for now this uses a brute-force solution while I work on other
 * parts of the project.
 */
struct lp_cell* get_cell(struct lp_grid* g, struct pixel* p)
{
    for (long x = 0; x < g->width; x++) {
        for (long y = 0; y < g->height; y++) {
            struct lp_cell* c = &(g->grid[x][y]);
            double diam = c->radius * 2;
            double cell_left = c->loc.x - c->radius;
            double cell_right = cell_left + diam;
            double cell_top = c->loc.y - c->radius;
            double cell_bot = cell_top + diam;

            if (cell_left < p->loc.x && p->loc.x < cell_right
                && cell_top < p->loc.y && p->loc.y < cell_bot) {
                return &(g->grid[x][y]);
            }
        }
    }

    // this should never happen since this should only be run on cells within
    // the overlay
    struct lp_cell* lpc = malloc(sizeof(struct lp_cell));

    lpc->loc.x = 1;
    lpc->loc.y = 2;
    lpc->dist = 0;
    lpc->npixels = -1;
    lpc->radius = 0;
    lpc->r = 0;
    lpc->g = 0;
    lpc->b = 0;

    return lpc;
}
