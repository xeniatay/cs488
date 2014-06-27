#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"
#include "ray.hpp"

class Primitive {
  public:
    virtual ~Primitive();
    virtual bool hit(Ray& r, Intersect& intersect);
};

class Sphere : public Primitive {
  public:
    virtual ~Sphere();
};

class Cube : public Primitive {
  public:
    virtual ~Cube();
    virtual bool hit(Ray& r, Intersect& intersect);
};

class NonhierSphere : public Primitive {
  public:
    NonhierSphere(const Point3D& pos, double radius): m_pos(pos), m_radius(radius) {}
    virtual ~NonhierSphere();

    virtual bool hit(Ray& r, Intersect& intersect);

  private:
    Point3D m_pos;
    double m_radius;

};

class NonhierBox : public Primitive {
  public:
    NonhierBox(const Point3D& pos, double size)
      : m_pos(pos), m_size(size)
    {
    }

    virtual ~NonhierBox();
    virtual bool hit(Ray& r, Intersect& intersect);

  private:
    Point3D m_pos;
    double m_size;
};

#endif
