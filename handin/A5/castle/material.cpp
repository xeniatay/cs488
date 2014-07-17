#include "material.hpp"
#include <GL/gl.h>
#include <GL/glu.h>

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd) : m_kd(kd) {
}

PhongMaterial::~PhongMaterial() { }

void PhongMaterial::apply_gl() {
  // Perform OpenGL calls necessary to set up this material.
  //std::cerr << "Apply Material: " << m_kd << std::endl;
  glColor4d( m_kd.R(), m_kd.G(), m_kd.B(), 1.0);
}
