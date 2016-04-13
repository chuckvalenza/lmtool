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
static png_infop info = NULL;
png_uint_32  width, height;
int bit_depth, color_type;

void readpng_version_info(void);

int png_init(FILE* infile, long* pWidth, long* pHeight);

int readpng_get_bgcolor(unsigned char *bg_red, unsigned char *bg_green,
    unsigned char *bg_blue);

unsigned char* readpng_get_image(double display_exponent, int *pChannels,
    unsigned long *pRowbytes);

void readpng_cleanup(int free_image_data);

#endif
