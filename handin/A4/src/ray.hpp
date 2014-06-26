#ifndef CS488_RAY_HPP
#define CS488_RAY_HPP

#include "algebra.hpp"
#include <vector>

using std::vector;

class Ray {
  public:
    Ray() : m_dir( Vector3D(0, 0, 0) ), m_origin( Point3D(0, 0, 0) ), hit(false) {};
    Ray(Vector3D& dir, Point3D& origin) : m_dir( dir ), m_origin( origin ), hit(false) {};

    Vector3D m_dir;
    Vector3D m_origin_vec;
    Point3D m_origin;
    bool hit;
    vector <double> roots;
};

#endif