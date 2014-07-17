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

using namespace std;

extern GLfloat ctrlpoints[4][4][3];
extern GLfloat texpts[2][2][2];
extern GLuint tex_count;

class Texture {
  public:
    enum Mode {
      IMAGE,
      PERLIN,
      NONE
    };

    enum TexId {
      CASTLE_WALL,
      GRASS
    };

    Mode m_mode;
    TexId m_texid;
    string m_filename;
    int m_h, m_w;

    virtual ~Texture();
    Texture(Mode mode, TexId texid, int h, int w, string filename);

    GLubyte img[3*imageWidth*imageHeight];
    Image *img_png;

    void init();

    void map_texture();
    void map_surface();

    void build_texture();
    void load_image(string filename, int w, int h);

    void display();
    void texture_test();
    void surface_test();

};


#endif