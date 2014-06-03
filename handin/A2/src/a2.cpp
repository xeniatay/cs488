// Xenia Tay
// 20396769
// xzytay

#include "a2.hpp"
#include <math.h>

// Return a matrix to represent a counterclockwise rotation of "angle"
// degrees around the axis "axis", where "axis" is one of the
// characters 'x', 'y', or 'z'.
Matrix4x4 rotation(double angle, char axis)
{
  Matrix4x4 r;

  char x = 'x', y = 'y', z = 'z';

  if (axis == x) {
    r[1][1] = cos(angle);
    r[1][2] = -1 * sin(angle);
    r[2][1] = sin(angle);
    r[2][2] = cos(angle);
  } else if (axis == y) {
    r[0][0] = cos(angle);
    r[0][2] = sin(angle);
    r[2][0] = -1 * sin(angle);
    r[2][2] = cos(angle);
  } else if (axis == z) {
    r[0][0] = cos(angle);
    r[0][1] = -1 * sin(angle);
    r[1][0] = sin(angle);
    r[1][1] = cos(angle);
  }

  return r;
}

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 translation(const Vector3D& displacement)
{
  Matrix4x4 t;

  // Subst the delta x and delta y values
  t[0][3] = displacement[0];
  t[1][3] = displacement[1];
  //v[2][3] = displacement[2];

  return t;
}

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 scaling(const Vector3D& scale)
{
  Matrix4x4 s;

  s[0][1] = scale[0];
  s[1][1] = scale[1];
  s[2][2] = scale[2];

  return s;
}
