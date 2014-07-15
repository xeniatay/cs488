#include "material.hpp"
#include <GL/gl.h>
#include <GL/glu.h>

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess, int texture)
  : m_kd(kd), m_ks(ks), m_shininess(shininess), m_texture(texture)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl() const
{
  // Perform OpenGL calls necessary to set up this material.
  //std::cerr << "Apply Material: " << m_kd << std::endl;
  if (m_texture == -1) {
    glColor4d( m_kd.R(), m_kd.G(), m_kd.B(), 1.0);
  }
}
