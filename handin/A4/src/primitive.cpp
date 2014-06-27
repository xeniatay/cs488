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
  double smallest_root = 0.0;

  size_t hits = quadraticRoots(A, B, C, roots);

  for (size_t i = 0; i < hits; i++) {
    // get the smallest (nearest) root
    if ( (roots[i] > 0) && (roots[i] < intersect.t) ) {
      smallest_root = roots[i];
    }
  }

  if (smallest_root > 0) {
    r.hit = true;

    // get intersection point
    intersect.t = smallest_root;
    intersect.m_ipoint = r.m_origin + (smallest_root * r.m_dir);

    // get normal
    intersect.m_normal = ( 1 / m_radius ) * ( intersect.m_ipoint - m_pos );
  }

  return r.hit;
}

NonhierBox::~NonhierBox()
{
}

bool NonhierBox::hit(Ray& r, Intersect& intersect) {

  // set boundaries for intersection
  double intersect_far = 500000.0;
  double intersect_near = -500000.0;
  int num_planes = 3;

  // for each face of cube, check for intersection using clipping method
  for (int i = 0; i < num_planes; i++) {

    // get both intersection points on current face (axis) of cube
    double m_ipoint_front = (double)( (m_pos[i] - r.m_origin[i]) / r.m_dir[i] );
    double m_ipoint_back = (double)( (m_pos[i] + m_size - r.m_origin[i]) / r.m_dir[i] );

    // check for front/back swapping
    if (m_ipoint_front > m_ipoint_back) {
      double temp = m_ipoint_front;
      m_ipoint_front = m_ipoint_back;
      m_ipoint_back = temp;
    }

    // if back intersection point is nearer than far plane
    if (m_ipoint_back < intersect_far) {
      // set ipoint as new far plane
      intersect_far = m_ipoint_back;
    }

    // if front intersection point is further than near plane
    if (m_ipoint_front > intersect_near) {

      // set normal for ipoint and normalize it
      intersect.m_normal = Vector3D(0, 0, 0);
      intersect.m_normal[i] = -r.m_dir[i];
      intersect.m_normal.normalize();

      // set ipoint as new near plane
      intersect_near = m_ipoint_front;

    }

    // check if front and back have overlapped, which means no intersection
    if ( (intersect_far < intersect_near) || (intersect_far < 0) ) {
      return r.hit; // r.hit is false
    }
  }

  // get ipoint
  double epsilon = 0.001;
  intersect.m_ipoint = Point3D(0, 0, 0);
  for (int i = 0; i < num_planes; i++) {
    intersect.m_ipoint[i] = r.m_dir[i] * (intersect_near - epsilon) + r.m_origin[i];
  }

  // set dist and hit
  intersect.t = intersect_near - epsilon;
  r.hit = true;

  return r.hit;
}

