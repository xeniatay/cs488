#include "primitive.hpp"
#include <cmath>
#include "polyroots.hpp"

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

  double roots[2];

  size_t hits = quadraticRoots(A, B, C, roots);

  if (hits) {
    r.hit = true;
    cerr << "HIT!" << endl;
  }

  for (int i = 0; i < hits; i++) {
    r.roots.push_back(roots[i]);
    cerr << "rootindex: " << i << ", rootval: " << roots[i] << endl;
  }

  return r;
}

NonhierBox::~NonhierBox()
{
}

Ray NonhierBox::hit(Ray& r) {
  return r;
}

