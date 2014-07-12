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
  std::cerr << "cube is drawn" << std::endl;

  glBegin(GL_QUADS);

    r_val = fmod((r + 0.4), 1);
    //multicolour ? glColor4d( r_val, g, b, a) : glColor4d(r, g, b, a);
    glVertex3d (x, y, z); // bottom left (front)
    glVertex3d (x, y + 1, z); // top left (front)
    glVertex3d (x + 1, y + 1, z); // top right (front)
    glVertex3d (x + 1, y, z); // bottom right (front)

    r_val = fmod((r + 0.8), 1);
    //multicolour ? glColor4d( r_val, g, b, a) : glColor4d(r, g, b, a);
    glVertex3d (x, y, z + 1); // bottom left (back)
    glVertex3d (x, y + 1, z + 1); // top left (back)
    glVertex3d (x + 1, y + 1, z + 1); // top right (back)
    glVertex3d (x + 1, y, z + 1); // bottom right (back)

    g_val = fmod((g + 0.4), 1);
    //multicolour ? glColor4d( r, g_val, b, a) : glColor4d(r, g, b, a);
    glVertex3d (x, y, z); // bottom left (left)
    glVertex3d (x, y + 1, z); // top left (left)
    glVertex3d (x, y + 1, z + 1); // top right (left)
    glVertex3d (x, y, z + 1); // bottom right (left)

    g_val = fmod((r + 0.8), 1);
    //multicolour ? glColor4d( r, g_val, b, a) : glColor4d(r, g, b, a);
    glVertex3d (x + 1, y, z); // bottom left (right)
    glVertex3d (x + 1, y + 1, z); // top left (right)
    glVertex3d (x + 1, y + 1, z + 1); // top right (right)
    glVertex3d (x + 1, y, z + 1); // bottom right (right)

    b_val = fmod((b + 0.4), 1);
    //multicolour ? glColor4d( r, g, b_val, a) : glColor4d(r, g, b, a);
    glVertex3d (x, y + 1, z); // bottom left (top)
    glVertex3d (x, y + 1, z + 1); // top left (top)
    glVertex3d (x + 1, y + 1, z + 1); // top right (top)
    glVertex3d (x + 1, y + 1, z); // bottom right (top)

    b_val = fmod((b + 0.8), 1);
    //multicolour ? glColor4d( r, g, b_val, a) : glColor4d(r, g, b, a);
    glVertex3d (x, y, z); // bottom left (bottom)
    glVertex3d (x, y, z + 1); // top left (bottom)
    glVertex3d (x + 1, y, z + 1); // top right (bottom)
    glVertex3d (x + 1, y, z); // bottom right (bottom)
  glEnd();
}
