#ifndef CS488_CASTLETERRAIN_HPP
#define CS488_CASTLETERRAIN_HPP

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define imageWidth 64
#define imageHeight 64

extern GLfloat ctrlpoints[4][4][3];
extern GLfloat textpts[2][2][2];
extern GLubyte ct_image[3*imageWidth*imageHeight];

void ct_display(void);
void ct_makeImage(void);
void ct_init(void);
void ct_reshape(int w, int h);
void ct_draw();

#endif