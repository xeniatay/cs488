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

Texture::Texture(Mode mode, TexId texid, int h, int w, string filename, double freq, double amp)
  : m_mode(mode), m_texid(texid), m_h(h), m_w(w), m_filename(filename), m_freq(freq), m_amp(amp) {

  m_init = false;

}

Texture::~Texture() {
}

void Texture::build_texture() {
  int i, j;
  double color;

  img = new GLubyte[3 * m_h * m_w];

  for (i = 0; i < m_w; i++) {
    for (j = 0; j < m_h; j++) {
      img[3*(m_h*i+j)] = (GLubyte) (8 * perlin2d(i, j)) + ( 4 * perlin2d(3 * i, 3 * j));
    }
  }

}

void Texture::load_image() {
  cerr << "loading " << m_filename << endl;
  //img_png = new Image(m_w, m_h, 3);
  //img_png->loadPng(m_filename);

  const char *c = m_filename.c_str();

  ImageBMP *image = loadBMP(c);
  load_texture(image);

  // SOIL can't link
  /*
    GLuint myTexture;
    char* fn = "assets/castle_wall_texture_3.png";
    myTexture = SOIL_load_OGL_texture( fn, 0, 1,
                       SOIL_FLAG_POWER_OF_TWO |
                       SOIL_FLAG_INVERT_Y );
                       */
}

void Texture::init() {

  // only init texture once
  if (!m_init) {
    if (m_mode == IMAGE) {

      const char *c = m_filename.c_str();

      ImageBMP *image = loadBMP(c);
      m_bindid = load_texture(image);

      //load_image();
    } else if (m_mode == PERLIN) {

      // allocate a texture
      glGenTextures( tex_count, &m_bindid );

      if (m_texid == SKY) {
        cerr << "SKYYY" << endl;
        makeSkyTexture(m_freq, m_amp);
      } else {
        make3DNoiseTexture(m_freq, m_amp);
      }
      //build_texture();
    }

    // map texture
    map_texture();

    m_init = true;
  }
}

void Texture::apply_gl() {
  // if texture hasn't been inited, init it
  init();

  if (m_mode == NONE) {
    return;
  } else if (m_mode == IMAGE) {
    // activate our current texture
    glBindTexture( GL_TEXTURE_2D, m_bindid );
    glDisable(GL_TEXTURE_3D);
    glEnable(GL_TEXTURE_2D);
  } else {
    glBlendFunc(GL_ONE, GL_ZERO);
    glBindTexture( GL_TEXTURE_3D, m_bindid );
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_3D);
  }

}

// from: http://www.nullterminator.net/gltexture.html
// and http://www.glprogramming.com/red/chapter12.html
void Texture::map_texture() {

  // mipmaps
  if (m_mode == IMAGE) {
    cerr << "start image texture" ;
    /*
    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the original
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // the texture wraps over at the edges (repeat)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, m_w, m_h, GL_RGBA, GL_UNSIGNED_BYTE, img_png->data());
    img_png->~Image();
    */
    cerr << "end" << endl;
  } else if (m_mode == PERLIN) {
    cerr << "start perlin texture" ;
    //gluBuild2DMipmaps( GL_TEXTURE_2D, 3, m_w, m_h, GL_RGB, GL_UNSIGNED_BYTE, img);
    init3DNoiseTexture();
    cerr << "end" << endl;
  }
}

void Texture::map_surface() {

  cerr << "map surface" << endl;

  // not mipmaps
  if (m_mode == IMAGE) {
    /*
    // Surface uses decal
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // the texture wraps over at the edges (repeat)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    // Surface doesn't use bilinear?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, m_w, m_h, 0, GL_RGB, GL_UNSIGNED_BYTE, img_png->data());
    */
  } else if (m_mode == PERLIN) {
    init3DNoiseTexture();
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, m_w, m_h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
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

GLuint load_texture(ImageBMP* image) {
  GLuint textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
  return textureId;
}