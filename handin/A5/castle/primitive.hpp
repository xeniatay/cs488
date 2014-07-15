#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP
#include <GL/gl.h>
#include <GL/glu.h>

#include "algebra.hpp"

class Primitive {
public:
  virtual ~Primitive();
  void draw_cube(double x, double y, double z, double r, double g, double b, double a);
  virtual void walk_gl(int texture);

  bool has_texture;
};

class Sphere : public Primitive {
public:
  Sphere();
  virtual ~Sphere();
  void walk_gl(int texture);
  GLuint dl;
};

class Cube : public Primitive {
public:
  Cube();
  virtual ~Cube();
  void draw_cube(double x, double y, double z, double r, double g, double b, double a);
  void draw_face(Matrix4x4 coords, Matrix4x4 texcoords, double r, double g, double b, double a);
  void walk_gl(int texture);
  GLuint dl_cube;
};

#endif
