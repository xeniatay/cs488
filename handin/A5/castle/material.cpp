#include "material.hpp"

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double transparent, int texture)
  : m_kd(kd), m_ks(ks), m_transparent(transparent), m_texture(texture)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl() const
{
  // Perform OpenGL calls necessary to set up this material.
  //std::cerr << "Apply Material: " << m_kd << std::endl;
  glColor4d( m_kd.R(), m_kd.G(), m_kd.B(), m_transparent);
}
