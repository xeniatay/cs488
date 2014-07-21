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

  m_scale = Vector3D(1, 1, 1);

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

  m_scale = Vector3D(m_scale[0] * amount[0], m_scale[1] * amount[1], m_scale[2] * amount[2]);
}

void SceneNode::translate(const Vector3D& amount)
{
  std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
  m_trans = m_trans * translation(amount);
}

void SceneNode::dimensions(int w, int h, int z) {
  cerr << "Error: Dimensions called on SceneNode" << endl;
  exit(1);
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

void GeometryNode::dimensions(int w, int h, int b) {
  cerr << "Dimensions called on GeoNode" << endl;
  m_primitive->m_w = w;
  m_primitive->m_h = h;
  m_primitive->m_b = b;
}

void GeometryNode::walk_gl(bool picking)
{

  //cerr << "GeometryNode " << m_name << " Walk GL" << endl;

  bool has_texture = false;

  glPushMatrix();
  gl_mult_trans(); // Convert transform matrix into a gl_float and apply it

  // enable alpha blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(0); // Disable writing to depth buffer

  //cerr << "material applied to " << m_name << endl;
  if (m_material) {
    PhongMaterial* p_material = (PhongMaterial*) m_material;
    p_material->apply_gl();
  }

  //cerr << "texture applied to " << m_name << endl;
  if (m_texture != NULL) {
    m_texture->apply_gl();
    has_texture = true;
  } else {
    has_texture = false;
  }

  // render primitive
  this->m_primitive->walk_gl(has_texture, m_scale);

  // render children primitive
  for( std::list<SceneNode*>::const_iterator i = m_children.begin(); i != m_children.end(); ++i ) {
    SceneNode *node = (*i);
    node->walk_gl();
  }

  // unbind and disable texture
  glBindTexture( GL_TEXTURE_2D, NULL);
  glBindTexture( GL_TEXTURE_3D, NULL);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_3D);

  // disable alpha blending
  glDisable(GL_BLEND);
  glDepthMask(1); // Re-enable writing to depth buffer

  glPopMatrix();

  //cerr << "GeometryNode " << m_name << " End Walk GL" << endl;
  cerr << m_name << " drawn | ";
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