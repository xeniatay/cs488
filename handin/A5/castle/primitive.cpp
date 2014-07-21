#include "primitive.hpp"

using std::cerr;
using std::endl;

Primitive::~Primitive() {
}

Sphere::~Sphere() {
}

Sphere::Sphere() {

  // display list
  dl = glGenLists(1);

}

void Sphere::walk_gl(bool texture, Vector3D scale) {
  //cerr << "Sphere Walk GL" << endl;

  //glNewList(dl, GL_COMPILE);

/*
  GLUquadric *quadric = gluNewQuadric();
  gluQuadricOrientation(quadric, GLU_OUTSIDE);
  gluSphere(quadric, 1, 32, 32);
  */

  double p_radius = 1;
  GLdouble a[] = {1, 0, 0};
  GLdouble b[] = {0, 0, -1};
  GLdouble c[] = {-1, 0, 0};
  GLdouble d[] = {0, 0, 1};
  GLdouble e[] = {0, 1, 0};
  GLdouble f[] = {0, -1, 0};

  int recurse = 5;

  draw_sphereface(recurse, p_radius, d, a, e);
  draw_sphereface(recurse, p_radius, a, b, e);
  draw_sphereface(recurse, p_radius, b, c, e);
  draw_sphereface(recurse, p_radius, c, d, e);
  draw_sphereface(recurse, p_radius, a, d, f);
  draw_sphereface(recurse, p_radius, b, a, f);
  draw_sphereface(recurse, p_radius, c, b, f);
  draw_sphereface(recurse, p_radius, d, c, f);

  glEndList();

  //glShadeModel(GL_SMOOTH);

  //glCallList (dl);

  has_texture = texture;
  m_scale = scale;

  //cerr << "Sphere End Walk GL" << endl;
}

inline void Normalize3(GLdouble *v)
{
   GLdouble len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
   v[0] /= len;
   v[1] /= len;
   v[2] /= len;
}

void Sphere::draw_sphereface(int p_recurse, double p_radius, GLdouble *a, GLdouble *b, GLdouble *c) {
  if(p_recurse > 1) {
    // Compute vectors halfway between the passed vectors
    GLdouble d[3] = {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
    GLdouble e[3] = {b[0] + c[0], b[1] + c[1], b[2] + c[2]};
    GLdouble f[3] = {c[0] + a[0], c[1] + a[1], c[2] + a[2]};

    Normalize3(d);
    Normalize3(e);
    Normalize3(f);

    draw_sphereface(p_recurse-1, p_radius, a, d, f);
    draw_sphereface(p_recurse-1, p_radius, d, b, e);
    draw_sphereface(p_recurse-1, p_radius, f, e, c);
    draw_sphereface(p_recurse-1, p_radius, f, d, e);
  }

   glBegin(GL_TRIANGLES);
      glNormal3dv(a);
      glVertex3d(a[0] * p_radius, a[1] * p_radius, a[2] * p_radius);
      glNormal3dv(b);
      glVertex3d(b[0] * p_radius, b[1] * p_radius, b[2] * p_radius);
      glNormal3dv(c);
      glVertex3d(c[0] * p_radius, c[1] * p_radius, c[2] * p_radius);
   glEnd();
}

Cube::~Cube() {
}

Cube::Cube() {

  // display list
  dl_cube = glGenLists(1);

  m_w = 1;
  m_h = 1;
  m_b = 1;

}

void Primitive::walk_gl(bool texture, Vector3D scale){ }

void Cube::walk_gl(bool texture, Vector3D scale)
{
  //cerr << "Cube Walk GL" << endl;

  glNewList(dl_cube, GL_COMPILE);

  //GLUquadric *quadric = gluNewQuadric();
  //gluQuadricOrientation(quadric, GLU_OUTSIDE);

  has_texture = texture;
  m_scale = scale;

  double x1 = 0;
  double x2 = 1;
  double y1 = 0;
  double y2 = 1;

  //cerr << "w, h, b: " << m_w << ", " << m_h << ", "<<m_b<<endl;
  for (int i = 0; i < m_w; i++) {
    for (int j = 0; j < m_h; j++) {
      for (int k = 0; k < m_b; k++) {
        draw_cube(i, j, k);
      }
    }
  }

  glEndList();

  glShadeModel(GL_SMOOTH);

  glCallList (dl_cube);


  //cerr << "Cube End Walk GL" << endl;
}

// Params: x, y z coords and r, g, b colour values
// Draws a unit cube (width = 1, height = 1, breadth = 1)
void Cube::draw_cube(double x = 0, double y = 0, double z = 2) {
  //bool multicolour = true;
  Matrix4x4 facecoords, texcoords;
  Vector4D bl, tl, tr, br;

  // read shader.txt
  read_shader();

  // Cel-Shading Code //
  /*
  glDisable(GL_TEXTURE_3D);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_TEXTURE_1D);                 // Enable 1D Texturing
  glBindTexture(GL_TEXTURE_1D, shaderTexture[0]);      // Bind Our Texture
  */

  // texcoords
  Vector4D tbl(0, 0, 0, 0);
  Vector4D ttl(0, 1, 0, 0);
  Vector4D ttr(1, 1, 0, 0);
  Vector4D tbr(1, 0, 0, 0);
  texcoords = Matrix4x4(tbl, ttl, ttr, tbr);

  //cerr << "texcoords: " << endl << texcoords << endl;

  glBegin(GL_QUADS);

    // front face
    bl = Vector4D(x, y, z, 0);
    tl = Vector4D(x, y + 1, z, 0);
    tr = Vector4D(x + 1, y + 1, z, 0);
    br = Vector4D(x + 1, y, z, 0);
    facecoords = Matrix4x4(bl, tl, tr, br);
    draw_face(facecoords, texcoords);

    // back face
    bl = Vector4D(x, y, z + 1, 0);
    tl = Vector4D(x, y + 1, z + 1, 0);
    tr = Vector4D(x + 1, y + 1, z + 1, 0);
    br = Vector4D(x + 1, y, z + 1, 0);
    facecoords = Matrix4x4(bl, tl, tr, br);
    draw_face(facecoords, texcoords);

    // left face
    bl = Vector4D(x, y, z, 0);
    tl = Vector4D(x, y + 1, z, 0);
    tr = Vector4D(x, y + 1, z + 1, 0);
    br = Vector4D(x, y, z + 1, 0);
    facecoords = Matrix4x4(bl, tl, tr, br);
    draw_face(facecoords, texcoords);

    // right face
    bl = Vector4D(x + 1, y, z, 0);
    tl = Vector4D(x + 1, y + 1, z, 0);
    tr = Vector4D(x + 1, y + 1, z + 1, 0);
    br = Vector4D(x + 1, y, z + 1, 0);
    facecoords = Matrix4x4(bl, tl, tr, br);
    draw_face(facecoords, texcoords);

    // top face
    bl = Vector4D(x, y + 1, z, 0);
    tl = Vector4D(x, y + 1, z + 1, 0);
    tr = Vector4D(x + 1, y + 1, z + 1, 0);
    br = Vector4D(x + 1, y + 1, z, 0);
    facecoords = Matrix4x4(bl, tl, tr, br);
    draw_face(facecoords, texcoords);

    // bottom face
    bl = Vector4D(x, y, z, 0);
    tl = Vector4D(x, y, z + 1, 0);
    tr = Vector4D(x + 1, y, z + 1, 0);
    br = Vector4D(x + 1, y, z, 0);
    facecoords = Matrix4x4(bl, tl, tr, br);
    draw_face(facecoords, texcoords);

  glEnd();

  // end cel shading
  /*
    glDepthFunc(GL_LESS); // Reset The Depth-Testing Mode
    glCullFace(GL_BACK); // Reset The Face To Be Culled
    glPolygonMode(GL_BACK, GL_FILL); // Reset Back-Facing Polygon Drawing Mode
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    */
}

void Cube::draw_face(Matrix4x4 coords, Matrix4x4 texcoords) {
  // multicolour
  //glColor4d(r, g, b, a);

  Vector3D v1 = Vector3D(coords[3][0], coords[3][1], coords[3][2]);
  Vector3D v2 = Vector3D(coords[1][0], coords[1][1], coords[1][2]);
  Vector3D cross = v1.cross(v2);
  //cerr << v1 << ", " << v2 << ", " << cross << " | ";
  GLdouble normal[] = {cross[0], cross[1], cross[2]};

  // calculate lighting stuff from http://tomdalling.com/blog/modern-opengl/06-diffuse-point-lighting/

  Vector3D lightPos = Vector3D{-300 - coords[3][0], 800 - coords[3][1], 1000 - coords[3][2]}; // hardccoded, please change
  lightPos.normalize();
  double tmpShade = cross.dot(lightPos);

  if (tmpShade < 0.0) {
    tmpShade = 0.0;
  }
  glTexCoord1f(tmpShade);

  int lightIndex = (int)tmpShade;
  double gray = shaderData[lightIndex][1];
  if (gray > 32) {
    gray = 32;
  }

  glColor4d( gray, gray, gray, 1.0);

  // bottom left
  if (has_texture) {
    glTexCoord2d(texcoords[0][0], texcoords[0][1]);
  }
  glNormal3dv(normal);
  glVertex3d(coords[0][0], coords[0][1], coords[0][2]);

  // top left
  if (has_texture) {
    glTexCoord2d(texcoords[1][0], texcoords[1][1]);
  }
  glNormal3dv(normal);
  glVertex3d(coords[1][0], coords[1][1], coords[1][2]);

  // top right
  if (has_texture) {
    glTexCoord2d(texcoords[2][0], texcoords[2][1]);
  }
  glVertex3d(coords[2][0], coords[2][1], coords[2][2]);
  glNormal3dv(normal);

  // bottom right
  if (has_texture) {
    glTexCoord2d(texcoords[3][0], texcoords[3][1]);
  }
  glNormal3dv(normal);
  glVertex3d(coords[3][0], coords[3][1], coords[3][2]);

}

bool Primitive::read_shader() {

  int i;                            // Looping Variable
  char Line[255];                       // Storage For 255 Characters

  FILE *In  = NULL;                     // File Pointer

  //g_window  = window;
  //g_keys  = keys;

  // Start Of User Initialization
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);     // Realy Nice perspective calculations

  glClearColor(0.7f, 0.7f, 0.7f, 0.0f);            // Light Grey Background
  glClearDepth(1.0f);                    // Depth Buffer Setup

  glEnable(GL_DEPTH_TEST);                 // Enable Depth Testing
  glDepthFunc(GL_LESS);                    // The Type Of Depth Test To Do

  glShadeModel(GL_SMOOTH);                 // Enables Smooth Color Shading
  glDisable(GL_LINE_SMOOTH);                 // Initially Disable Line Smoothing

//  glEnable(GL_CULL_FACE);                  // Enable OpenGL Face Culling

  glDisable(GL_LIGHTING);                  // Disable OpenGL Lighting

  In = fopen("./Shader.txt", "r");            // Open The Shader File

  if(In) {
    for(i = 0; i < 32; i++) {
      if(feof(In)) {
        break;
      }

      fgets(Line, 255, In);                // Get The Current Line
      shaderData[i][0] = shaderData[i][1] = shaderData[i][2] = double(atof(Line)); // Copy Over The Value
    }
    fclose(In);                      // Close The File
  } else {
    return false;                     // It Went Horribly Horribly Wrong
  }

/*
  glGenTextures(1, &shaderTexture[0]);           // Get A Free Texture ID
  glBindTexture(GL_TEXTURE_1D, shaderTexture[0]);      // Bind This Texture. From Now On It Will Be 1D

  // For Crying Out Loud Don't Let OpenGL Use Bi/Trilinear Filtering!
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 32, 0, GL_RGB , GL_FLOAT, shaderData);  // Upload

  lightAngle.X = 0.0f;                    // Set The X Direction
  lightAngle.Y = 0.0f;                    // Set The Y Direction
  lightAngle.Z = 1.0f;                    // Set The Z Direction

  Normalize(lightAngle); // Normalize The Light Direction
*/
  return false;
}