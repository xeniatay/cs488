#include "primitive.hpp"
#include <cmath>
#include "polyroots.hpp"

using std::cerr;
using std::endl;

Primitive::~Primitive()
{
}

bool Primitive::hit(Ray& r, Intersect& intersect) {
  return r.hit;
}

Sphere::~Sphere()
{
}

Cube::~Cube()
{
}

bool Cube::hit(Ray& r, Intersect& intersect) {
  return r.hit;
}

NonhierSphere::~NonhierSphere()
{
}

bool NonhierSphere::hit(Ray& r, Intersect& intersect) {

  Vector3D orig_minus_pos = r.m_origin - m_pos;

  double A = r.m_dir.dot(r.m_dir);
  double B = 2 * (orig_minus_pos).dot(r.m_dir);
  double C = (orig_minus_pos).dot(orig_minus_pos) - (m_radius * m_radius);

  double roots[2];

  size_t hits = quadraticRoots(A, B, C, roots);

  for (int i = 0; i < hits; i++) {
    // get the smallest (nearest) root
    if ( (roots[i] > 0) && (roots[i] < intersect.t) ) {
      r.hit = true;

      // get intersection point
      intersect.t = roots[i];
      intersect.m_ipoint = r.m_origin + (roots[i] * r.m_dir);

      // get normal
      intersect.m_normal = intersect.m_ipoint - m_pos;

    }
  }

  return r.hit;
}

NonhierBox::~NonhierBox()
{
}

bool NonhierBox::hit(Ray& r, Intersect& intersect) {
  return r.hit;
}

