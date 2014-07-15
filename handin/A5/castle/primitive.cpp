#include "primitive.hpp"
#include <iostream>

using std::cerr;
using std::endl;

Primitive::~Primitive() {
}

Sphere::~Sphere() {
}

Sphere::Sphere() {

  // display list
  dl = glGenLists(1);

}

void Sphere::walk_gl(bool picking) {
  //cerr << "Sphere Walk GL" << endl;

  glNewList(dl, GL_COMPILE);

  GLUquadric *quadric = gluNewQuadric();
  gluQuadricOrientation(quadric, GLU_OUTSIDE);

  gluSphere(quadric, 1, 32, 32);

  glEndList();

  glShadeModel(GL_SMOOTH);

  glCallList (dl);

  //cerr << "Sphere End Walk GL" << endl;
}

Cube::~Cube() {
}

Cube::Cube() {

  // display list
  dl_cube = glGenLists(1);

}

void Primitive::walk_gl(bool picking){ }

void Cube::walk_gl(bool picking)
{
  //cerr << "Cube Walk GL" << endl;

  glNewList(dl_cube, GL_COMPILE);

  //GLUquadric *quadric = gluNewQuadric();
  //gluQuadricOrientation(quadric, GLU_OUTSIDE);

  draw_cube(0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0);

  glEndList();

  glShadeModel(GL_SMOOTH);

  glCallList (dl_cube);

  //cerr << "Cube End Walk GL" << endl;
}

// Params: x, y z coords and r, g, b colour values
// Draws a unit cube (width = 1, height = 1, breadth = 1)
void Cube::draw_cube(double x = 0, double y = 0, double z = 2, double r = 0, double g = 0, double b = 0, double a = 0.8) {
  //bool multicolour = (m_mode == MULTICOLOURED);
  bool multicolour = true;
  double r_val = 0, g_val = 0, b_val = 0;
  Matrix4x4 facecoords, texcoords;
  Vector4D bl, tl, tr, br;
  std::cerr << "cube is drawn" << std::endl;

  // map texture to shape
  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, 1);

  Vector4D tbl(0, 0, 0, 0);
  Vector4D ttl(0, 10, 0, 0);
  Vector4D ttr(10, 10, 0, 0);
  Vector4D tbr(10, 0, 0, 0);
  texcoords = Matrix4x4(tbl, ttl, ttr, tbr);

  glBegin(GL_QUADS);

    // front face
    bl = Vector4D(x, y, z, 0);
    tl = Vector4D(x, y + 1, z, 0);
    tr = Vector4D(x + 1, y + 1, z, 0);
    br = Vector4D(x + 1, y, z, 0);
    facecoords = Matrix4x4(bl, tl, tr, br);
    draw_face(facecoords, texcoords, r, g, b, a);

    // back face
    bl = Vector4D(x, y, z + 1, 0);
    tl = Vector4D(x, y + 1, z + 1, 0);
    tr = Vector4D(x + 1, y + 1, z + 1, 0);
    br = Vector4D(x + 1, y, z + 1, 0);
    facecoords = Matrix4x4(bl, tl, tr, br);
    draw_face(facecoords, texcoords, r, g, b, a);

    // left face
    bl = Vector4D(x, y, z, 0);
    tl = Vector4D(x, y + 1, z, 0);
    tr = Vector4D(x, y + 1, z + 1, 0);
    br = Vector4D(x, y, z + 1, 0);
    facecoords = Matrix4x4(bl, tl, tr, br);
    draw_face(facecoords, texcoords, r, g, b, a);

    // right face
    bl = Vector4D(x + 1, y, z, 0);
    tl = Vector4D(x + 1, y + 1, z, 0);
    tr = Vector4D(x + 1, y + 1, z + 1, 0);
    br = Vector4D(x + 1, y, z + 1, 0);
    facecoords = Matrix4x4(bl, tl, tr, br);
    draw_face(facecoords, texcoords, r, g, b, a);

    // top face
    bl = Vector4D(x, y + 1, z, 0);
    tl = Vector4D(x, y + 1, z + 1, 0);
    tr = Vector4D(x + 1, y + 1, z + 1, 0);
    br = Vector4D(x + 1, y + 1, z, 0);
    facecoords = Matrix4x4(bl, tl, tr, br);
    draw_face(facecoords, texcoords, r, g, b, a);

    // bottom face
    bl = Vector4D(x, y, z, 0);
    tl = Vector4D(x, y, z + 1, 0);
    tr = Vector4D(x + 1, y, z + 1, 0);
    br = Vector4D(x + 1, y, z, 0);
    facecoords = Matrix4x4(bl, tl, tr, br);
    draw_face(facecoords, texcoords, r, g, b, a);

  glEnd();
}

void Cube::draw_face(Matrix4x4 coords, Matrix4x4 texcoords, double r, double g, double b, double a) {
  //multicolour ? glColor4d( r_val, g, b, a) : glColor4d(r, g, b, a);

  // bottom left
  glTexCoord2d(texcoords[0][0], texcoords[0][1]);
  glVertex3d(coords[0][0], coords[0][1], coords[0][2]);

  // top left
  glTexCoord2d(texcoords[1][0], texcoords[1][1]);
  glVertex3d(coords[1][0], coords[1][1], coords[1][2]);

  // top right
  glTexCoord2d(texcoords[2][0], texcoords[2][1]);
  glVertex3d(coords[2][0], coords[2][1], coords[2][2]);

  // bottom right
  glTexCoord2d(texcoords[3][0], texcoords[3][1]);
  glVertex3d(coords[3][0], coords[3][1], coords[3][2]);

}