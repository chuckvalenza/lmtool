/**
 * lmtool.c
 * 
 * Author: Chuck Valenza
 */

#include "lmtool.h"

/**
 * simply print the usage of the tool
 */
void print_usage()
{
    char* args = "<input img> <output img> <out height> <out width>";

    fprintf(stdout, "Usage: lmtool %s\n", args);

    exit(-1);
}

/**
 * Encapsulate the tiff data into the lpt's pixel struct
 */
struct pixel** tiff_to_img(TIFF* infile, long* height, long* width)
{
    struct pixel** pix_grid;

    if (!infile) {
        fprintf(stderr, "Failed to load file\n");
        exit(-1);
    }

    uint32 in_w;
    uint32 in_h;
    size_t npixels;
    uint32* raster;

    TIFFGetField(infile, TIFFTAG_IMAGEWIDTH, &in_w);
    TIFFGetField(infile, TIFFTAG_IMAGELENGTH, &in_h);

    *width = in_w;
    *height = in_h;

    npixels = in_w * in_h;
    raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));

    if (!raster) {
        TIFFClose(infile);
        fprintf(stderr, "Error reading tiff file\n");
        exit(FAIL_BAD_FILE);
    }

    if (TIFFReadRGBAImage(infile, in_w, in_h, raster, 0)) {
        int raster_pos;
        char p;

        pix_grid = malloc(sizeof(struct pixel*) * in_h);

        for (int row = 0; row < in_h; row++) {
            pix_grid[row] = malloc(sizeof(struct pixel) * in_w);

            for (int col = 0; col < in_w; col++) {
                pix_grid[row][col].x = col;
                pix_grid[row][col].y = row;

                // since this is greyscale, rg&b are all the same value
                pix_grid[row][col].r = raster[row * in_w + col] % 256;
                pix_grid[row][col].g = raster[row * in_w + col] % 256;
                pix_grid[row][col].b = raster[row * in_w + col] % 256;
            }

        }
    }

    _TIFFfree(raster);

    TIFFClose(infile);

    return pix_grid;
}

/**
 * Write the data to the output image
 */
int grid_to_tiff(TIFF* outfile, struct lp_cell** grid, long out_w, long out_h)
{
    if (!outfile) {
        fprintf(stderr, "Failed to open output file.\n");
        exit(-1);
    }

    uint32* raster = malloc(sizeof(uint32) * out_w * out_h);

    TIFFSetField(outfile, TIFFTAG_IMAGEWIDTH, &out_w);
    TIFFSetField(outfile, TIFFTAG_IMAGELENGTH, &out_h);

    // construct the raster
    for (int row = 0; row < out_w; row++) {
        for (int col = 0; col < out_h; col++) {
            uint32 val = (uint32)(grid[row][col].r / grid[row][col].npixels);
            raster[row * out_w + col] = val;
        }
    }

    TIFFClose(outfile);
}

/**
 * main flow of program
 */
int main(int argc, char** argv)
{
    // program setup stuff
    if (argc != (ARG_LENGTH + 1)) {
        print_usage();
    }

    long image_width;
    long image_height;
    long output_width = atol(argv[3]);
    long output_height = atol(argv[4]);
    TIFF* infile = TIFFOpen(argv[1], "r");
    TIFF* outfile = TIFFOpen(argv[2], "w");
    char fn_in[1024];
    char fn_out[1024];
    strcpy(fn_in, argv[1]);
    strcpy(fn_out, argv[2]);

    printf("%s -> %s, %ldx%ld\n", fn_in, fn_out, output_width, output_height);

    // start doing the actual meat & bones of implementation of the library
    struct pixel** image_data;
    struct lp_cell** lp_grid;

    image_data = tiff_to_img(infile, &image_width, &image_height);

    lp_allocate_grid(&lp_grid, output_width, output_height);

    // set up distances of each cell to the image
    lp_init_grid(&lp_grid, image_width / 2, output_width, output_height);

    lp_transform(image_data, &lp_grid, image_width, image_height, output_width,
        output_height);

    grid_to_tiff(outfile, lp_grid, output_width, output_height);

    return 0;
}
