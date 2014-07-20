#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP
#include <GL/glew.h>
#include <GL/glu.h>

#include "algebra.hpp"
#include "texture.hpp"
#include <iostream>

using namespace std;

class Primitive {
public:
  virtual ~Primitive();
  void draw_cube(double x, double y, double z, double r, double g, double b, double a);
  virtual void walk_gl(bool texture, Vector3D scale);

  bool has_texture;
  Vector3D m_scale;
};

class Sphere : public Primitive {
public:
  Sphere();
  virtual ~Sphere();
  void walk_gl(bool texture, Vector3D scale);
  GLuint dl;
};

class Cube : public Primitive {
public:
  Cube();
  virtual ~Cube();
  void draw_cube(double x, double y, double z, double r, double g, double b, double a);
  void draw_face(Matrix4x4 coords, Matrix4x4 texcoords, double r, double g, double b, double a);
  void walk_gl(bool texture, Vector3D scale);
  GLuint dl_cube;
};

#endif
