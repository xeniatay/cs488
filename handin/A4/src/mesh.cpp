#include "mesh.hpp"
#include <iostream>
#include <vector>

using std::vector;

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces)
  : m_verts(verts),
    m_faces(faces)
{
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  std::cerr << "mesh({";
  for (std::vector<Point3D>::const_iterator I = mesh.m_verts.begin(); I != mesh.m_verts.end(); ++I) {
    if (I != mesh.m_verts.begin()) std::cerr << ",\n      ";
    std::cerr << *I;
  }
  std::cerr << "},\n\n     {";

  for (std::vector<Mesh::Face>::const_iterator I = mesh.m_faces.begin(); I != mesh.m_faces.end(); ++I) {
    if (I != mesh.m_faces.begin()) std::cerr << ",\n      ";
    std::cerr << "[";
    for (Mesh::Face::const_iterator J = I->begin(); J != I->end(); ++J) {
      if (J != I->begin()) std::cerr << ", ";
      std::cerr << *J;
    }
    std::cerr << "]";
  }
  std::cerr << "});" << std::endl;
  return out;
}

bool Mesh::hit(Ray& r, Intersect& intersect) {

  bool is_intersect = false;
  Vector3D m_normal;
  Point3D m_ipoint;
  double intersect_dotprod;
  int epsilon = 0.001;

  // set boundaries for intersection
  double intersect_max = 500000.0;
  double intersect_min = 500000.0;

  // check for intersection on each face of mesh
  for ( vector<Face>::const_iterator i = m_faces.begin(); i != m_faces.end(); i++ ) {

    // get all all_vertices on current face
    int count = 0;
    is_intersect = false;
    Point3D all_vertices[ (*i).size() ];

    for ( Mesh::Face::const_iterator j = (*i).begin(); j != (*i).end(); j++ ) {
      all_vertices[count] = m_verts[*j];
      count++;
    }

    // get m_normal of face
    m_normal = (all_vertices[2] - all_vertices[1]).cross(all_vertices[0] - all_vertices[1]);
    m_normal.normalize();

    // if intersection is outside boundaries or is not closest intersection, skip this iteration
    intersect_dotprod = m_normal.dot( all_vertices[0] - r.m_origin ) / m_normal.dot( r.m_dir );
    if ( (intersect_dotprod < epsilon) || (intersect_dotprod > intersect_min) ) {
      continue;
    }

    // set intersection point
    m_ipoint = r.m_origin + intersect_dotprod * r.m_dir;

    // use intersection of halfspaces to check if current intersection is within face
    for (int j = 0; j < (*i).size(); j++) {
      int hs_index = (j + 1) % (*i).size();
      Vector3D vertex1 = all_vertices[hs_index] - all_vertices[j];
      Vector3D vertex2 = m_ipoint - all_vertices[j];
      double intersect_halfspace = vertex1.cross(vertex2).dot(m_normal);

      if ( intersect_halfspace >= 0 ) {
        is_intersect = true;
      } else {
        is_intersect = false;
        break;
      }
    }

    // if current ray within face boundaries, set intersection and new min
    if (is_intersect) {
      intersect_min = intersect_dotprod;
      intersect.m_normal = m_normal;
      intersect.m_ipoint = m_ipoint;
      intersect.dist = intersect_dotprod;
    }
  }

  // if min != max, there is a hit
  if ( intersect_min != intersect_max ) {
    r.hit = true;
  }

  return r.hit;
}