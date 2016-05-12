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
                pix_grid[row][col].loc.x = col;
                pix_grid[row][col].loc.y = row;

                // since this is greyscale, rg&b are all the same value
                pix_grid[row][col].r = raster[row * in_w + col] % 255;
                pix_grid[row][col].g = raster[row * in_w + col] % 255;
                pix_grid[row][col].b = raster[row * in_w + col] % 255;
            }
        }
    }

    _TIFFfree(raster);

    TIFFClose(infile);

    return pix_grid;
}

uint32 avg(long v, long n)
{
    double ret = (double)v / (double)n;
    return (uint32)ret;
}

/**
 * Write the data to the output image
 */
int grid_to_tiff(struct lp_grid* g, TIFF* outfile)
{
    long out_w = g->width;
    long out_h = g->height;

    if (!outfile) {
        fprintf(stderr, "Failed to open output file.\n");
        exit(-1);
    }

    int bytes_per_pixel = 3;
    uint32 rowsperstrip = TIFFDefaultStripSize(outfile, -1); 

    TIFFSetField(outfile, TIFFTAG_IMAGEWIDTH, out_w);
    TIFFSetField(outfile, TIFFTAG_IMAGELENGTH, out_h);
    TIFFSetField(outfile, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
    TIFFSetField(outfile, TIFFTAG_SAMPLESPERPIXEL, bytes_per_pixel);
    TIFFSetField(outfile, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(outfile, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(outfile, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(outfile, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(outfile, TIFFTAG_ROWSPERSTRIP,
        TIFFDefaultStripSize(outfile, out_w * bytes_per_pixel));

    size_t linebytes = bytes_per_pixel * out_w;

    // construct the raster
    for (uint32 row = 0; row < out_w; row++) {
        unsigned char* buffer;

        buffer = (unsigned char*)_TIFFmalloc(linebytes);

        for (int col = 0; col < out_h; col++) {
            buffer[3 * col] = avg(g->grid[row][col].r,
                g->grid[row][col].npixels);
            buffer[3 * col + 1] = avg(g->grid[row][col].g,
                g->grid[row][col].npixels);
            buffer[3 * col + 2] = avg(g->grid[row][col].b,
                g->grid[row][col].npixels);
        }

        if (TIFFWriteScanline(outfile, buffer, row, 0) < 0) {
            break;
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
    struct lp_grid grid_obj;

    image_data = tiff_to_img(infile, &image_width, &image_height);

    lp_create_grid(&grid_obj, output_width, output_height);

    // set up distances of each cell to the image
    lp_init_grid(&grid_obj, image_width / 2);

    lp_transform(&grid_obj, image_data, image_width, image_height);

    grid_to_tiff(&grid_obj, outfile);

    return 0;
}
