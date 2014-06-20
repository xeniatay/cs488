#include "primitive.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>

using std::cerr;
using std::endl;

Primitive::~Primitive() {
}

Sphere::~Sphere() {
}

void Sphere::walk_gl(bool picking) const
{
  cerr << "Sphere Walk GL" << endl;

  // display list
  GLuint listName = glGenLists(1);

  glNewList(listName, GL_COMPILE);

  GLUquadric *quadric = gluNewQuadric();
  gluQuadricOrientation(quadric, GLU_OUTSIDE);

  gluSphere(quadric, 1, 32, 32);

  glEndList();

  glShadeModel(GL_FLAT);

  glCallList (listName);

  cerr << "Sphere End Walk GL" << endl;
}

