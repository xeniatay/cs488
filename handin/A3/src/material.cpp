#include "material.hpp"
#include <GL/gl.h>
#include <GL/glu.h>

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess)
  : m_kd(kd), m_ks(ks), m_shininess(shininess)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl() const
{
  // Perform OpenGL calls necessary to set up this material.
  glColor4d( m_kd.R(), m_kd.G(), m_kd.B(), 1.0);
  //std::cerr << "Apply Material: " << m_kd.R() << ", " << m_kd.G() << ", " << m_kd.B() << std::endl;
}
