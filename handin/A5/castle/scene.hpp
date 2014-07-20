#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include "algebra.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include "texture.hpp"
#include <GL/glew.h>
#include <GL/glu.h>

class SceneNode {
  public:
    SceneNode(const std::string& name);
    virtual ~SceneNode();

    virtual void walk_gl(bool picking = false);

    const Matrix4x4& get_transform() const { return m_trans; }
    const Matrix4x4& get_inverse() const { return m_invtrans; }

    void set_transform(const Matrix4x4& m)
    {
      m_trans = m;
      m_invtrans = m.invert();
    }

    void set_transform(const Matrix4x4& m, const Matrix4x4& i)
    {
      m_trans = m;
      m_invtrans = i;
    }

    void add_child(SceneNode* child)
    {
      m_children.push_back(child);

      //std::cerr << "SceneNode " << this->m_name << " add child" << std::endl;
    }

    void remove_child(SceneNode* child)
    {
      m_children.remove(child);
    }

    // Callbacks to be implemented.
    // These will be called from Lua.
    void rotate(char axis, double angle);
    void scale(const Vector3D& amount);
    void translate(const Vector3D& amount);

    // Returns true if and only if this node is a JointNode
    virtual bool is_joint();

    // Useful for picking
    int m_id;
    int m_geo_id;
    std::string m_name;

    // scaling for texture
    Vector3D m_scale;

    void printTM();

    void gl_mult_trans();

    // Transformations
    Matrix4x4 m_trans;
    Matrix4x4 m_invtrans;
    Matrix4x4 m_rot;
    GLfloat m_gl_trans[16];

    // Hierarchy
    typedef std::list<SceneNode*> ChildList;
    ChildList m_children;
};

class JointNode : public SceneNode {
  public:
    JointNode(const std::string& name);
    virtual ~JointNode();

    virtual void walk_gl(bool bicking = false);

    virtual bool is_joint();

    void set_joint_x(double min, double init, double max);
    void set_joint_y(double min, double init, double max);

    struct JointRange {
      double min, init, max;
    };


    JointRange m_joint_x, m_joint_y;
};

class GeometryNode : public SceneNode {
  public:
    GeometryNode(const std::string& name, Primitive* primitive);
    virtual ~GeometryNode();

    virtual void walk_gl(bool picking = false);

    const Material* get_material() const;
    Material* get_material();

    void set_material(Material* material) { m_material = material; }
    void set_texture(Texture* texture) { m_texture = texture; }

    Material* m_material;
    Texture* m_texture;
    Primitive* m_primitive;

    void transform_node();

};

#endif
