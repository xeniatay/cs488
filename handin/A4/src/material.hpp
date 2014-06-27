#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"

class Material {
public:
  virtual ~Material();
  virtual void apply_gl() const = 0;

  Material() { }
  protected:

};

class PhongMaterial : public Material {
public:
  PhongMaterial(const Colour& kd, const Colour& ks, double shininess);
  virtual ~PhongMaterial();

  virtual void apply_gl() const;

  Colour get_kd() {
    return m_kd;
  }

  Colour get_ks() {
    return m_ks;
  }

  double get_shininess() {
    return m_shininess;
  }

  Colour m_kd;
  Colour m_ks;

  double m_shininess;

private:
};


#endif
