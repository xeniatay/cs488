#include "scene.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include "a2.hpp"

using std::cerr;
using std::endl;

typedef std::list<SceneNode*> SN;
SN all_scenenodes;

SceneNode::SceneNode(const std::string& name) : m_name(name) {

  this->m_id = all_scenenodes.size() + 1;
  transform = Matrix4x4();

  all_scenenodes.push_back(this);

  //cerr << "Init sceneNode: " << name << " " << this->m_id << endl;
}

SceneNode::~SceneNode() { }

void SceneNode::walk_gl(bool picking) const
{
  cerr << "SceneNode Walk GL" << endl;

  for( std::list<SceneNode*>::const_iterator i = m_children.begin(); i != m_children.end(); ++i ) {
    SceneNode *node = (*i);
    node->walk_gl();
  }

}

void SceneNode::rotate(char axis, double angle)
{
  std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;

  transform = transform * rotation(90, axis);
}

void SceneNode::scale(const Vector3D& amount)
{
  std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;

  transform = transform * scaling(amount);
}

void SceneNode::translate(const Vector3D& amount)
{
  std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;

  transform = transform * translation(amount);
}

bool SceneNode::is_joint() const
{
  return false;
}

JointNode::JointNode(const std::string& name) : SceneNode(name) {
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl(bool picking) const
{
  // Fill me in
}

bool JointNode::is_joint() const
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

}

GeometryNode::~GeometryNode() {
}

void GeometryNode::walk_gl(bool picking) const
{
  cerr << "GeometryNode Walk GL" << endl;
  m_material->apply_gl();
  // Fill me in
  this->m_primitive->walk_gl(false);
}