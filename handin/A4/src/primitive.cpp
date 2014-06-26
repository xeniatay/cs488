#include "primitive.hpp"

class Ray;

Primitive::~Primitive()
{
}

Ray Primitive::hit(Ray& r) {
  return r;
}

Sphere::~Sphere()
{
}

Ray Sphere::hit(Ray& r) {

/*
  double A = (b - a) * (b - a);
  double B = (b - a) * (a - c);
  double C = (a - c) * (a - c) - (m_radius * m_radius);
*/

  //A = r->dir->dot(r->dir) rayDirection dot rayDirection.  B = 2 * (rayOrigin - sphere pos) dot rayDirection. C = (rayOrigin - sphere pos) dot (rayOrigin - sphere pos) - r^2


  return r;
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
  return r;
}

NonhierBox::~NonhierBox()
{
}

Ray NonhierBox::hit(Ray& r) {
  return r;
}

