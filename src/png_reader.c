/**
 * png_reader.c
 * 
 * Author: Chuck Valenza
 */

#include <stdio.h>
#include "png_reader.h"

/**
 * This will set up the headers of the png image
 */
int png_init(FILE* infile, long* pWidth, long* pHeight)
{
    unsigned char sig[8];

    // check for a bad signature in the file; ensures this is a PNG file
    fread(sig, 1, 8, infile);
    if(!png_check_sig(sig, 8))
        return 1;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
        NULL, NULL);
    
    if(!png_ptr) {
        return 4;
    }

    info = png_create_info_struct(png_ptr);

    if(!info) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 4;
    }

    if(setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info, NULL);
        return 2;
    }

    // set up the io files for libpng to write to
    png_init_io(png_ptr, infile);

    // skip the 8 signature bytes since we already read them
    png_set_sig_bytes(png_ptr, 8);

    // read info about the contents of the image file format
    png_read_info(png_ptr, info);

    png_get_IHDR(png_ptr, info, &width, &height, &bit_depth, &color_type,
        NULL, NULL, NULL);
    *pWidth = width;
    *pHeight = height;

    return 0;
}
