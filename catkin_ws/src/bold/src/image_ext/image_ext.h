#ifndef IMAGE_EXT_H
#define IMAGE_EXT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void write_eps( double * segs, int n, int dim, char * filename, int xsize, int ysize, double width );
double* char_to_image_double_ptr( unsigned int xsize, unsigned int ysize, char * data );

#endif
