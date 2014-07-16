#ifndef CS488_CASTLETERRAIN_HPP
#define CS488_CASTLETERRAIN_HPP

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <string>
#include <iostream>
#include "image.hpp"
#include "perlinnoise.hpp"

#define imageWidth 64
#define imageHeight 64

extern GLfloat ctrlpoints[4][4][3];
extern GLfloat texpts[2][2][2];
extern GLuint tex_count;

class Texture {
  public:
    virtual ~Texture();
    Texture();

    GLubyte img[3*imageWidth*imageHeight];
    Image *img_png;

    int img_h, img_w;

    void display();
    void makeImage();
    void init();
    void map_texture();
    void texture_test();

    // GLUT window
    void draw();
    void reshape(int w, int h);
};


#endif