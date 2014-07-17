#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"
#include "texture.hpp"
#include <string>

using namespace std;

class Material {
  public:
    virtual ~Material();

  protected:
    Material() { }
};

class PhongMaterial : public Material {
  public:

    PhongMaterial(const Colour& kd);
    virtual ~PhongMaterial();

    void apply_gl();

    Colour m_kd;

  private:
};


#endif
