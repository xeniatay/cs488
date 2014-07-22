#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP
#include <GL/glew.h>
#include <GL/glu.h>

#include "algebra.hpp"
#include "texture.hpp"
#include <iostream>
#include <string>
#include "celshading.hpp"
#include "glm.h"

using namespace std;

class Primitive {
public:
  Primitive();
  virtual ~Primitive();
  void draw_cube(double x, double y, double z, double r, double g, double b, double a);
  virtual void walk_gl(bool texture, Vector3D scale);

  bool has_texture;
  Vector3D m_scale;

  int m_w, m_h, m_b;

  void init_celshading();
  void destruct_celshading();
  void get_celshading(Vector3D normal);
  bool m_celshading;

  void walk_gl_castle(char *filename);

};

class Sphere : public Primitive {
public:
  Sphere();
  virtual ~Sphere();
  void draw_sphereface(int p_recurse, double p_radius, GLdouble *a, GLdouble *b, GLdouble *c);
  void walk_gl(bool texture, Vector3D scale);
  GLuint dl;
};

class Cube : public Primitive {
public:
  Cube();
  virtual ~Cube();
  void draw_cube(double x, double y, double z);
  void draw_face(Matrix4x4 coords, Matrix4x4 texcoords);
  void walk_gl(bool texture, Vector3D scale);
  void draw_cube_outline(double x, double y, double z);
  GLuint dl_cube;
};

#endif
