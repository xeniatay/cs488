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

void Sphere::walk_gl(bool picking) const
{
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

