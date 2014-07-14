/*
 * file: pngLoad.h
 * author: Michael Norton
 * from: http://www.macdevcenter.com/pub/a/mac/2005/10/14/texture-maps.html
 *
 * Wrapper code for libpng to read a png file.
 * Code is based heavily on the examples found in
 * the OReilly PNG book.
 * Found online at:
 * http://www.libpng.org/pub/png/book/chapter13.html#png.ch13.div.1
 *
 */
#ifndef PNGLOAD_H
#define PNGLOAD_H

#include <png.h>

/* exported function */
int pngLoad(char *file, unsigned long **pwidth,
  unsigned long **pheight, char **image_data_ptr);

#endif /* PNGLOAD_H */