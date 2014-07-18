#include "texture.hpp"

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

Texture::Texture(Mode mode, TexId texid, int h, int w, string filename)
  : m_mode(mode), m_texid(texid), m_h(h), m_w(w), m_filename(filename) {

  m_init = false;

}

Texture::~Texture() {
}

void Texture::build_texture() {
  int i, j;
  double color;

  GLubyte perlin[3 * m_h * m_w];

  for (i = 0; i < m_w; i++) {
    for (j = 0; j < m_h; j++) {
      perlin[3*(m_h*i+j)] = (GLubyte) (8 * perlin2d(i, j)) + ( 4 * perlin2d(3 * i, 3 * j));
    }
  }

  img = perlin;
}

void Texture::load_image() {
  img_png = new Image(m_w, m_h, 3);
  img_png->loadPng(m_filename);
}

void Texture::init() {

  // only init texture once
  if (!m_init) {
    cerr << "texture " << m_texid << " is inited" << endl;
    if (m_mode == IMAGE) {
      load_image();
    } else if (m_mode == PERLIN) {
      build_texture();
    }

    // map texture
    map_texture();

    m_init = true;
  }
}

void Texture::apply_gl() {
  // allocate a texture
  GLuint texture = (GLuint)m_texid;
  glGenTextures( tex_count, &texture );

  // activate our current texture
  glBindTexture( GL_TEXTURE_2D, texture );

  // if texture hasn't been inited, init it
  init();
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

  // mipmaps
  if (m_mode == IMAGE) {
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, m_w, m_h, GL_RGB, GL_UNSIGNED_BYTE, img_png->data());
  } else if (m_mode == PERLIN) {
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, m_w, m_h, GL_RGB, GL_UNSIGNED_BYTE, img);
  }
}

void Texture::map_surface() {

  cerr << "map surface" << endl;

  // Surface uses decal
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  // the texture wraps over at the edges (repeat)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  // Surface doesn't use bilinear?
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  build_texture();

  // not mipmaps
  if (m_mode == IMAGE) {
    glTexImage2D(GL_TEXTURE_2D, 0, 3, m_w, m_h, 0, GL_RGB, GL_UNSIGNED_BYTE, img_png->data());
  } else if (m_mode == PERLIN) {
    glTexImage2D(GL_TEXTURE_2D, 0, 3, m_w, m_h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
  }

  // enable texture stuff
  //glEnable(GL_TEXTURE_2D);
  //glEnable(GL_DEPTH_TEST);
  //glShadeModel (GL_FLAT);

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

  // display shape
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(0.0, 1.0, 0.6);
  glEvalMesh2(GL_FILL, 0, 20, 0, 20);
  glFlush();
}
