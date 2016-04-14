/**
 * png_reader.c
 * 
 * Author: Chuck Valenza
 */

#include <stdio.h>
#include "png_reader.h"

/**
 * Load the headers into the image pointer
 */
int png_init(FILE* infile, long* pWidth, long* pHeight)
{
    unsigned char sig[8];

    // check for a bad signature in the file; ensures this is a PNG file
    fread(sig, 1, 8, infile);
    if (!png_check_sig(sig, 8))
        return 1;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
        NULL, NULL);
    
    if (!png_ptr) {
        return 1;
    }

    info_ptr = png_create_info_struct(png_ptr);

    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 1;
    }

    // set up the io files for libpng to write to
    png_init_io(png_ptr, infile);

    // skip the 8 signature bytes since we already read them
    png_set_sig_bytes(png_ptr, 8);

    // read info about the contents of the image file format
    png_read_info(png_ptr, info_ptr);

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    *pWidth = width;
    *pHeight = height;

    return 0;
}

/**
 * Load the background color from the file and load it to our image pointer
 */
int png_load_bg_color(unsigned char *red, unsigned char *green,
    unsigned char *blue)
{
    png_color_16p pBackground;

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 1;
    }

    if (!png_get_valid(png_ptr, info_ptr, PNG_INFO_bKGD)) {
        return 1;
    }

    png_get_bKGD(png_ptr, info_ptr, &pBackground);

    if (bit_depth == 16) {
        *red = pBackground->red >> 8;
        *green = pBackground->green >> 8;
        *blue = pBackground->blue >> 8;
    } else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        if (bit_depth == 1) {
            *red = *green = *blue = pBackground->gray ? 255 : 0;
        } else if (bit_depth == 2) {
            *red = *green = *blue = (255 / 3) * pBackground->gray;
        } else {
            *red = *green = *blue = (255 / 15) * pBackground->gray;
        }
    } else {
        *red = (unsigned char)pBackground->red;
        *green = (unsigned char)pBackground->green;
        *blue = (unsigned char)pBackground->blue;
    }

    return 0;
}
