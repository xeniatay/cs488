#ifndef CS488_RAY_HPP
#define CS488_RAY_HPP

#include "algebra.hpp"
#include "material.hpp"
#include <vector>

using std::vector;

class Ray {
  public:
    Ray() : m_dir( Vector3D(0, 0, 0) ), m_origin( Point3D(0, 0, 0) ), hit(false),
            m_kd(0, 0, 0), m_ks(0, 0, 0), m_ke(0, 0, 0), m_ambient(0, 0, 0) {};
    Ray(Vector3D& dir, Point3D& origin) : m_dir( dir ), m_origin( origin ), hit(false),
            m_kd(0, 0, 0), m_ks(0, 0, 0), m_ke(0, 0, 0), m_ambient(0, 0, 0) {};

    Vector3D m_dir;
    Vector3D m_origin_vec;
    Point3D m_origin;
    bool hit;
    vector <double> roots;
    int num_bounces;

    Colour m_kd;
    Colour m_ks;
    Colour m_ke;
    Colour m_ambient;
};

class Intersect {
    public:
        Intersect() {
            t = 50000;
        };

        Vector3D m_normal;
        Point3D m_ipoint;
        PhongMaterial* m_material;
        double t, dist;
};

#endif