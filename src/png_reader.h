/**
 * png_reader.c
 * 
 * Author: Chuck Valenza
 */

#ifndef PNG_READER_H
#define PNG_READER_H

#include <stdio.h>
#include <stdlib.h>
#include "png.h"

static png_structp png_ptr = NULL;
static png_infop info_ptr = NULL;
png_uint_32  width, height;
int bit_depth, color_type;
unsigned char* png_image;

void readpng_version_info(void);

int png_init(FILE* infile, long* pWidth, long* pHeight);

int png_load_bg_color();

unsigned char* readpng_get_image(double display_exponent, int *pChannels,
    unsigned long *pRowbytes);

void readpng_cleanup(int free_image_data);

#endif
