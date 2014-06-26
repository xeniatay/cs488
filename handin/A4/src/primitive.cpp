#include "primitive.hpp"
#include <cmath>

using std::cerr;
using std::endl;

Primitive::~Primitive()
{
}

Ray Primitive::hit(Ray& r) {
  return r;
}

Sphere::~Sphere()
{
}

Cube::~Cube()
{
}

Ray Cube::hit(Ray& r) {
  return r;
}

NonhierSphere::~NonhierSphere()
{
}

Ray NonhierSphere::hit(Ray& r) {

  Vector3D orig_minus_pos = r.m_origin - m_pos;

  double A = r.m_dir.dot(r.m_dir);
  double B = 2 * (orig_minus_pos).dot(r.m_dir);
  double C = (orig_minus_pos).dot(orig_minus_pos) - (m_radius * m_radius);

  double t1 = (-2 * C) / (B + sqrt( ( B * B) - (4 * A * C) ) );
  double t2 = (-2 * C) / (B - sqrt( ( B * B) - (4 * A * C) ) );

  cerr << "t1: " << t1 << ", t2: " << t2 << endl;

  return r;
}

NonhierBox::~NonhierBox()
{
}

Ray NonhierBox::hit(Ray& r) {
  return r;
}

