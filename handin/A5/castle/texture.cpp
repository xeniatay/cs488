#include "texture.hpp"

using namespace std;

GLfloat ctrlpoints[4][4][3] = {
   {{ -1.5, -1.5, 4.0}, { -0.5, -1.5, 2.0},
    {0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}},
   {{ -1.5, -0.5, 1.0}, { -0.5, -0.5, 3.0},
    {0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}},
   {{ -1.5, 0.5, 4.0}, { -0.5, 0.5, 0.0},
    {0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}},
   {{ -1.5, 1.5, -2.0}, { -0.5, 1.5, -2.0},
    {0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};

GLfloat texpts[2][2][2] = {{{0.0, 0.0}, {0.0, 1.0}},
                        {{1.0, 0.0}, {1.0, 1.0}}};

GLuint tex_count = 0;

Texture::Texture() {
  img_h = 64;
  img_w = 64;
}

Texture::~Texture() {
}

void Texture::display(void) {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);
   glEvalMesh2(GL_FILL, 0, 20, 0, 20);
   glFlush();
}

void Texture::makeImage() {
  int i, j;
  float ti, tj;

  for (i = 0; i < img_w; i++) {
    ti = 2.0*3.14159265*i/img_w;
    for (j = 0; j < img_h; j++) {
      /*
      tj = 2.0*3.14159265*j/img_h;
      img[3*(img_h*i+j)] = (GLubyte) 127*(1.0+sin(ti));
      img[3*(img_h*i+j)+1] = (GLubyte) 127*(1.0+cos(2*tj));
      img[3*(img_h*i+j)+2] = (GLubyte) 127*(1.0+cos(ti+tj));
      */
      img[3*(img_h*i+j)] = (GLubyte) perlin2d(i, j);
      img[3*(img_h*i+j)+1] = (GLubyte) perlin2d(i, j);
      img[3*(img_h*i+j)+2] = (GLubyte) perlin2d(i, j);
    }
  }

/*
  img = new Image(img_w, img_h, 3);

  for (i = 0; i < img_w; i++) {
    ti = 2.0*3.14159265*i/img_w;
    for (j = 0; j < img_h; j++) {
      tj = 2.0*3.14159265*j/img_h;
      img->m_data[3*(img_h*i+j)] = (GLubyte) 127*(1.0+sin(ti));
      img->m_data[3*(img_h*i+j)+1] = (GLubyte) 127*(1.0+cos(2*tj));
      img->m_data[3*(img_h*i+j)+2] = (GLubyte) 127*(1.0+cos(ti+tj));
    }
  }
  */

  // hardcoded for now
  string filename = "assets/castle_wall_texture_1.png";
  img_png->loadPng(filename);

}

void Texture::init()
{
  GLuint texture;
  tex_count++;

  img_png = new Image(img_h, img_w, 3);

/*
  // resize img
  size_t img_size = 3 * img_w * img_h;
  img = new GLubyte[img_size];
  */
  // allocate a texture name
  glGenTextures( tex_count, &texture );

  // activate our current texture
  glBindTexture( GL_TEXTURE_2D, texture );

  surface_test();
  map_texture();
}

// from: http://www.nullterminator.net/gltexture.html
// and http://www.glprogramming.com/red/chapter12.html
void Texture::map_texture() {
  cerr << "map texture" << endl;

  // select modulate to mix texture with color for shading
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

  // when texture area is small, bilinear filter the closest mipmap
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
  // when texture area is large, bilinear filter the original
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  // the texture wraps over at the edges (repeat)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  makeImage();

  // mipmaps
  //gluBuild2DMipmaps( GL_TEXTURE_2D, 3, img_w, img_h, GL_RGB, GL_UNSIGNED_BYTE, img_png->data() );
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, img_w, img_h, GL_RGB, GL_UNSIGNED_BYTE, img );
}

void Texture::map_surface() {

  cerr << "map surface" << endl;

  // Surface uses decal
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  // the texture wraps over at the edges (repeat)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  // Surface doesn't use bilinear?
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  makeImage();

  // not mipmaps
  glTexImage2D(GL_TEXTURE_2D, 0, 3, img_w, img_h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
  //glTexImage2D(GL_TEXTURE_2D, 0, 3, img_w, img_h, 0, GL_RGB, GL_UNSIGNED_BYTE, img_png->data());

  // enable texture stuff
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glShadeModel (GL_FLAT);

}

void Texture::reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho(-4.0, 4.0, -4.0*(GLfloat)h/(GLfloat)w,
              4.0*(GLfloat)h/(GLfloat)w, -4.0, 4.0);
   else
      glOrtho(-4.0*(GLfloat)w/(GLfloat)h,
              4.0*(GLfloat)w/(GLfloat)h, -4.0, 4.0, -4.0, 4.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotatef(85.0, 1.0, 1.0, 1.0);
}

// glut window rendering that we don't need
void Texture::draw() {
  /*
  char *myargv [1];
  int myargc = 1;
  myargv[0] = "ct";

  glutInit(&myargc, myargv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (myargv[0]);
  Texture::init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
  */
}


void Texture::texture_test() {
  // map texture to shape
  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, 1);
  glBegin( GL_QUADS );
  glTexCoord2d(0.0,0.0); glVertex2d(0.0,0.0);
  glTexCoord2d(10.0,0.0); glVertex2d(10.0,0.0);
  glTexCoord2d(10.0,10.0); glVertex2d(10.0,10.0);
  glTexCoord2d(0.0,10.0); glVertex2d(0.0,10.0);
  glEnd();
}

void Texture::surface_test() {
  // enable surface
  glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
  glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texpts[0][0][0]);
  glEnable(GL_MAP2_TEXTURE_COORD_2);
  glEnable(GL_MAP2_VERTEX_3);
  glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
}