#include "a4.hpp"
#include "image.hpp"
#include "a2.hpp"

using std::cerr;
using std::endl;

void a4_render(// What to render
               SceneNode* root,
               // Where to output the image
               const std::string& filename,
               // Image size
               int width, int height,
               // Viewing parameters
               const Point3D& eye, const Vector3D& view,
               Vector3D& up, double fov,
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

  // bg colour calculations
  double bg_r = 0.2;
  double bg_g = 1.0;
  double bg_b = 0.63;

  double bg_r_diff = (bg_r - 0.1) / width;
  double bg_g_diff = (bg_g - 0.45) / width;
  double bg_b_diff = (bg_b - 0.33) / width;

  // iterate through image pixel by pixel and cast a ray

  Vector3D ray_dir;
  Vector3D pworld;
  Vector3D pk;
  Point3D lookfrom = eye;
  double d = 1500;

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

  int x = 1;
  int y = 1;
  for (int x = 0; x < width; x++) {
    bg_r += bg_r_diff;
    bg_g += bg_g_diff;
    bg_b += bg_b_diff;

    for (int y = 0; y < height; y++) {
      // use this ray to calculate the colour of the pixel
      pk[0] = x;
      pk[1] = y;
      pk[2] = 0;

      // STEP 1 make z = 0, so translate pk
      // STEP 2: scale to preserve aspect ratio and correct sign
      // STEP 3: rotate to superimpose WCS to VCS
      // STEP 4: translate by lookfrom vector
      // pworld = t4 * r3 * s2 * t1 * pk
      pworld = t4 * r3 * s2 * t1 * pk;
      ray_dir = pworld - lookfrom_vec;
      ray_dir.normalize();

      Ray r;
      r.m_dir = ray_dir;
      r.m_origin = lookfrom;
      r.m_origin_vec = lookfrom_vec;
      r = root->hit(r);

      // set background gradient (fuschia to black from bottom to top)
      bg_r = fmod( bg_r, 1.0 );
      bg_g = fmod( bg_g, 1.0 );
      bg_b = fmod( bg_b, 1.0 );
      Colour px_colour( bg_r, bg_g, bg_b );

      px_colour = ray_colour(r, lookfrom, px_colour, up);

      img(x, y, 0) = px_colour.R();
      img(x, y, 1) = px_colour.G();
      img(x, y, 2) = px_colour.B();
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

Ray ggReflection(Ray& r, Vector3D& N) {
  Vector3D v = r.m_origin_vec;
  double v_dot_N = v.dot(N);
  r.m_dir = v + (2 * v_dot_N) * N;

  return r;
}

bool colourIsZero(Colour& c) {
  return ( (c.R() != 0) && (c.G() != 0) && (c.B() != 0));
}

Colour ray_colour(Ray& r, Point3D& uv, Colour& bg, Vector3D &up) {
  Colour kd(1, 0, 0);
  Colour ks(0.5, 0.5, 0.5);
  Colour ke(0, 1, 0);
  Colour zero(0, 0, 0);

  Colour px_colour(0, 0, 0);

  if (r.hit) {
    px_colour = ke; // plus ambient

    if (!colourIsZero(kd)) {
      // check for other intersections
    }

    if (!colourIsZero(ks)) {
      r = ggReflection(r, up);
      px_colour = px_colour + (ks * ray_colour(r, uv, bg, up));
    }

    return px_colour;
  }

  return bg;
}

