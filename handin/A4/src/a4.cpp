#include "a4.hpp"
#include "image.hpp"
#include "a2.hpp"
#include "ray.hpp"

using std::cerr;
using std::endl;

class Ray;

void a4_render(// What to render
               SceneNode* root,
               // Where to output the image
               const std::string& filename,
               // Image size
               int width, int height,
               // Viewing parameters
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               // Lighting parameters
               const Colour& ambient,
               const std::list<Light*>& lights
               )
{
  // Fill in raytracing code here.

  std::cerr << "Stub: a4_render(" << root << ",\n     "
            << filename << ", " << width << ", " << height << ",\n     "
            << eye << ", " << view << ", " << up << ", " << fov << ",\n     "
            << ambient << ",\n     {";

  for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
    if (I != lights.begin()) std::cerr << ", ";
    std::cerr << **I;
  }
  std::cerr << "});" << std::endl;

  Image img(width, height, 3);

  // iterate through image pixel by pixel and cast a ray

  Vector3D ray_dir;
  Vector3D pworld;
  Vector3D pk;
  Point3D lookfrom = eye;
  double d = 2500;

  Matrix4x4 t1 = translation( Vector3D( -1 * (width / 2), -1 * (height / 2), d ) );

  double h = (2 * d) * tan(fov / 2);
  //double w = (width / height) * h;
  Matrix4x4 s2 = scaling( Vector3D( -1 * (h/height), (h/height), 1 ) );

  Vector3D w_r = view;
  w_r.normalize();
  Vector3D u_r = w_r.cross(up);
  u_r.normalize();
  Vector3D v_r = w_r.cross(u_r);
  Vector4D w_r4(w_r[0], w_r[1], w_r[2], 0);
  Vector4D u_r4(u_r[0], u_r[1], u_r[2], 0);
  Vector4D v_r4(v_r[0], v_r[1], v_r[2], 0);
  Vector4D zero_vec(0, 0, 0, 0);
  Matrix4x4 r3(u_r4, v_r4, w_r4, zero_vec);

  Vector3D lookfrom_vec(lookfrom[0], lookfrom[1], lookfrom[2]);
  Matrix4x4 t4 = translation(lookfrom_vec);

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      // use this ray to calculate the colour of the pixel
      Colour c(1, 0, 0);

      pk[0] = x;
      pk[1] = y;
      pk[2] = 0;

      // STEP 1 make z = 0, so translate pk
      // STEP 2: scale to preserve aspect ratio and correct sign
      // STEP 3: rotate to superimpose WCS to VCS
      // STEP 4: translate by lookfrom vector
      // pworld = t4 * r3 * s2 * t1 * pk
      Vector3D pworld = t4 * r3 * s2 * t1 * pk;
      ray_dir = pworld - lookfrom_vec;
      cerr << "Ray dir: " << endl << ray_dir << endl;

      Ray r();
      //root->hit(r);

    }
  }

  // For now, just make a sample image.
  /*
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < height; x++) {
      // Red: increasing from top to bottom
      img(x, y, 0) = (double)y / height;
      // Green: increasing from left to right
      img(x, y, 1) = (double)x / width;
      // Blue: in lower-left and upper-right corners
      img(x, y, 2) = ((y < height/2 && x < height/2)
                      || (y >= height/2 && x >= height/2)) ? 1.0 : 0.0;
    }
  } */

  img.savePng(filename);

}
