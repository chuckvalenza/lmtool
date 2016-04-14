/**
 * lmtool.c
 * 
 * Author: Chuck Valenza
 */

#include "lmtool.h"

void print_usage()
{
    char* args = "<img> <out height> <out width>";

    fprintf(stdout, "Usage: lmtool %s\n", args);

    exit(-1);
}

unsigned char* load_png(FILE* infile, long* pWidth, long* pHeight)
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
}

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

    char filename[1024];
    strcpy(filename, argv[1]);

    printf("%s %ld %ld\n", filename, output_width, output_height);

    load_png(infile, &image_width, &image_height);

    return 0;
}
