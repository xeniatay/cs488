#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include <GL/glew.h>
#include <GL/glu.h>
#include "algebra.hpp"
#include "glm.h"

class Material {
public:
  virtual ~Material();
  virtual void apply_gl() const = 0;

protected:
  Material()
  {
  }
};

class PhongMaterial : public Material {
public:
  PhongMaterial(const Colour& kd, const Colour& ks, double transparent, int texture);
  virtual ~PhongMaterial();

  virtual void apply_gl() const;

private:
  Colour m_kd;
  Colour m_ks;

  double m_transparent;
  int m_texture;
};


#endif
