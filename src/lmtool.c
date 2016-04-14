/**
 * lmtool.c
 * 
 * Author: Chuck Valenza
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "png_reader.h"

#define ARG_LENGTH 3

#define FAIL_BAD_FILE 80

void print_usage()
{
    char* args = "<img> <out height> <out width>";

    fprintf(stdout, "Usage: lmtool %s\n", args);

    exit(-1);
}

unsigned char* load_png(FILE* infile, long* pWidth, long* pHeight)
{
    if(png_init(infile, pWidth, pHeight) != 0) {
        exit(FAIL_BAD_FILE);
    }

    if(png_load_bg_color() != 0) {
        exit(FAIL_BAD_FILE);
    }
}

int main(int argc, char** argv)
{
    if(argc != (ARG_LENGTH + 1)) {
        print_usage();
    }

    long image_width;
    long image_height;
    long output_width = atol(argv[2]);
    long output_height = atol(argv[3]);
    long** image_data = NULL;
    FILE* infile = fopen(argv[1], "r");

    char filename[1024];
    strcpy(filename, argv[1]);

    printf("%s %ld %ld\n", filename, output_width, output_height);

    image_data = load_png(infile, &image_width, &image_height);

    return 0;
}
