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
    char* args = "<img> <out height> <out width>";

    fprintf(stdout, "Usage: lmtool %s\n", args);

    exit(-1);
}

/**
 * load the png into an array of unsigned chars to process
 */
unsigned char* load_png(FILE* infile, long* pWidth, long* pHeight,
    int* image_channels, unsigned long* image_rowbytes)
{
    if (readpng_init(infile, pWidth, pHeight) != 0) {
        fclose(infile);
        exit(FAIL_BAD_FILE);
    }

    unsigned char r, g, b;

    if (readpng_get_bgcolor(&r, &g, &b) != 0) {
        fclose(infile);
        exit(FAIL_BAD_FILE);
    }

    return readpng_get_image(image_channels, image_rowbytes);
}

/**
 * main flow of program
 */
int main(int argc, char** argv)
{
    if (argc != (ARG_LENGTH + 1)) {
        print_usage();
    }

    long image_width;
    long image_height;
    long output_width = atol(argv[2]);
    long output_height = atol(argv[3]);
    FILE* infile = fopen(argv[1], "r");

    unsigned char* image_data;
    unsigned long image_rowbytes;
    int image_channels;


    char filename[1024];
    strcpy(filename, argv[1]);

    printf("%s %ld %ld\n", filename, output_width, output_height);

    image_data = load_png(infile, &image_width, &image_height, &image_channels,
        &image_rowbytes);

    // free png data from memory, but not the image_data we just read in
    readpng_cleanup(0);

    struct lp_cell** lp_grid = allocate_lparray(output_width, output_height);

    lp_transform(image_data, &lp_grid, image_rowbytes, input_width,
        input_height, output_width, output_height);

    return 0;
}
