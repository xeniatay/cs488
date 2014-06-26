#include "scene.hpp"
#include <iostream>
#include "a2.hpp"
#include <vector>

using std::vector;
using std::cerr;
using std::endl;

SceneNode::SceneNode(const std::string& name) : m_name(name) {
  m_trans = Matrix4x4();
}

SceneNode::~SceneNode() {
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

bool SceneNode::is_joint() const {
  return false;
}

Ray SceneNode::hit(Ray& r) {

  bool intersect = false;

  for( std::list<SceneNode*>::const_iterator i = m_children.begin(); i != m_children.end(); ++i ) {
    SceneNode *node = (*i);
    r = node->hit(r);

    if (intersect) {
      return r;
    }

  };

  return r;

}

JointNode::JointNode(const std::string& name) : SceneNode(name) {
}

JointNode::~JointNode() {
}

bool JointNode::is_joint() const {
  return true;
}

void JointNode::set_joint_x(double min, double init, double max) {
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max) {
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive) : SceneNode(name),m_primitive(primitive) {

  //cerr << "Init geometryNode: " << name << endl;

  this->m_primitive = primitive;
  this->m_name = name;

}

GeometryNode::~GeometryNode() {
}

Ray GeometryNode::hit(Ray& r) {
  this->m_primitive->hit(r);
  return r;

  /*
  for( std::list<SceneNode*>::const_iterator i = m_children.begin(); i != m_children.end(); ++i ) {
    SceneNode *node = (*i);
    node->walk_gl();
  }
  */

}

