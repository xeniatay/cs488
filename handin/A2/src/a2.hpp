// Xenia Tay
// 20396769
// xzytay

#ifndef CS488_A2_HPP
#define CS488_A2_HPP

#include "algebra.hpp"

// You should implement these functions, and use them from viewer.cpp

// Return a matrix to represent a counterclockwise rotation of "angle"
// degrees around the axis "axis", where "axis" is one of the
// characters 'x', 'y', or 'z'.
Matrix4x4 rotation(double angle, char axis);

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 translation(const Vector3D& displacement);

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 scaling(const Vector3D& scale);

// Return a matrix to represent a reflection with the given axis.
Matrix4x4 reflection(char axis);

Matrix4x4 projection(double near, double far, double angle);
Point3D homogenizeProjection(Point3D p, double z);
Point3D reverseHomogenizeProjection(Point3D p, double z);

#endif
