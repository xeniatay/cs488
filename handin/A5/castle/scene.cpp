#include "scene.hpp"
#include <iostream>
#include "a2.hpp"
#include <vector>

using std::vector;
using std::cerr;
using std::endl;

vector<SceneNode*> scenenodes_v;

typedef std::list<SceneNode*> SN;
SN all_scenenodes;

typedef std::list<SceneNode*> GN;
GN all_geonodes;

SceneNode::SceneNode(const std::string& name) : m_name(name) {

  this->m_id = all_scenenodes.size() + 1;
  m_trans = Matrix4x4();

  all_scenenodes.push_back(this);
  scenenodes_v.push_back(this);

  //cerr << "Init sceneNode: " << name << " " << this->m_id << endl;
}

SceneNode::~SceneNode() { }

void SceneNode::walk_gl(bool picking)
{
  //cerr << "SceneNode " << m_name << " Walk GL" << endl;

  glPushMatrix();

  //cerr << "m_trans and rot " << endl << m_trans << endl << m_rot << endl;

  // Translate root
  gl_mult_trans();

  for( std::list<SceneNode*>::const_iterator i = m_children.begin(); i != m_children.end(); ++i ) {
    SceneNode *node = (*i);
    node->walk_gl();
  }

  glPopMatrix();

  //cerr << "SceneNode " << m_name << " End Walk GL" << endl;
}

void SceneNode::rotate(char axis, double angle)
{
  std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
  m_rot = m_rot * rotation(angle, axis);
  m_trans = m_trans * rotation(angle, axis);
}

void SceneNode::scale(const Vector3D& amount)
{
  std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
  m_trans = m_trans * scaling(amount);
}

void SceneNode::translate(const Vector3D& amount)
{
  std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
  m_trans = m_trans * translation(amount);
}

bool SceneNode::is_joint() {
  return false;
}

void SceneNode::printTM() {
  int i = 0;

  for (i = 0; i < 16; i++) {
    cerr << m_gl_trans[i] << ", " << endl;
  }

}

JointNode::JointNode(const std::string& name) : SceneNode(name) {
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl(bool picking)
{
  glPushMatrix();
  gl_mult_trans();

  for( std::list<SceneNode*>::const_iterator i = m_children.begin(); i != m_children.end(); ++i ) {
    SceneNode *node = (*i);
    node->walk_gl();
  }

  glPopMatrix();
}

bool JointNode::is_joint()
{
  return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive) : SceneNode(name), m_primitive(primitive) {

  //cerr << "Init geometryNode: " << name << endl;

  this->m_primitive = primitive;
  this->m_name = name;

  this->m_geo_id = all_geonodes.size();
  all_geonodes.push_back(this);

  this->m_texture = NULL;
  this->m_material = NULL;

}

GeometryNode::~GeometryNode() {
}

void GeometryNode::walk_gl(bool picking)
{

  //cerr << "GeometryNode " << m_name << " Walk GL" << endl;

  if (m_material) {
    PhongMaterial* p_material = (PhongMaterial*) m_material;
    p_material->apply_gl();
    //cerr << "material applied" << endl;
  }

  if (m_texture != NULL) {
    m_texture->apply_gl();
    //cerr << "texture applied" << endl;
  }

  glPushMatrix();
  gl_mult_trans();

  this->m_primitive->walk_gl(0);

  for( std::list<SceneNode*>::const_iterator i = m_children.begin(); i != m_children.end(); ++i ) {
    SceneNode *node = (*i);
    node->walk_gl();
  }

  glPopMatrix();

  // unbind texture
  glBindTexture( GL_TEXTURE_2D, NULL);

  //cerr << "GeometryNode " << m_name << " End Walk GL" << endl;
}

// make the transform matrix into a gl_float
void SceneNode::gl_mult_trans() {

  Matrix4x4 tr = m_trans;

  int i = 0, j = 0, index;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      index = (i * 4) + j;
      m_gl_trans[index] = tr[i][j];
    }
  }

  glMultTransposeMatrixf(m_gl_trans);

  //cerr << m_name << "'s GL transform matrix has been set:" << endl << tr << endl;
}

// apply transformations
void GeometryNode::transform_node() {


}