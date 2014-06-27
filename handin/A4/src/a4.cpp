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
  double d = eye[2];

  Matrix4x4 t1 = translation( Vector3D( -1 * (double)(width / 2), -1 * (double)(height / 2), (double)d ) );

  double h = 2 * d * tan(fov * M_PI / 360);
  double w = (width / height) * h;
  Matrix4x4 s2 = scaling( Vector3D( -1 * (h/height), (h/height), 1 ) );

  Vector3D w_rot = -1 * view;
  w_rot.normalize();
  Vector3D u_rot = w_rot.cross(up);
  u_rot.normalize();
  Vector3D v_rot = (w_rot).cross(u_rot);
  v_rot.normalize();
  Matrix4x4 r3(
    Vector4D(u_rot[0], v_rot[0], w_rot[0], 0),
    Vector4D(u_rot[1], v_rot[1], w_rot[1], 0),
    Vector4D(u_rot[2], v_rot[2], w_rot[2], 0),
    Vector4D(0, 0, 0, 0)
  );

  Vector3D lookfrom_vec(lookfrom[0], lookfrom[1], lookfrom[2]);
  Matrix4x4 t4 = translation(lookfrom_vec);

  Matrix4x4 m_pworld = t4 * r3 * s2 * t1;

/*
  cerr << "width, height: " << width << " " << height << endl;
  cerr << "h: " << h << ", w: " << w << endl;
  cerr << "t1: " << endl << t1 << endl;
  cerr << "s2: " << endl << s2 << endl;
  cerr << "r3: " << endl << r3 << endl;
  cerr << "t4: " << endl << t4 << endl;
  cerr << "m_pworld: " << endl << m_pworld << endl;
  */

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
      pworld = m_pworld * pk;
      ray_dir = pworld - lookfrom_vec;
      ray_dir.normalize();

      Ray r;
      r.m_dir = ray_dir;
      r.m_origin = lookfrom;
      r.m_origin_vec = lookfrom_vec;
      r.m_ambient = ambient;

      Intersect intersect;
      root->hit(r, intersect);

      // set background gradient (fuschia to black from bottom to top)
      bg_r = fmod( bg_r, 1.0 );
      bg_g = fmod( bg_g, 1.0 );
      bg_b = fmod( bg_b, 1.0 );
      Colour px_colour( bg_r, bg_g, bg_b );

      for (std::list <Light*>::const_iterator i = lights.begin(); i != lights.end(); ++i) {
        Light* light = (*i);
        px_colour = ray_colour(r, intersect, px_colour, light, root);
      }

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
  cerr << "ggreflection" << endl;
  Vector3D v = r.m_origin_vec;
  double v_dot_N = v.dot(N);
  r.m_dir = v + (2 * v_dot_N) * N;

  return r;
}

Colour direct_light(Intersect& intersect, Vector3D& l, SceneNode* root) {

  // fix dots by shifting ray a little bit
  Point3D new_intersect_point = intersect.m_ipoint + (0.001 * l);

  Ray sr(l, new_intersect_point);
  Intersect s_intersect;

  Colour darken(0.0, 0.0, 0.0);
  Colour no_change(1.0, 1.0, 1.0);

  root->hit(sr, s_intersect);
  if (sr.hit && (s_intersect.t > 0.001)) {
    return darken;
  }

  return no_change;
}

bool colourIsZero(Colour& c) {
  return ( (c.R() == 0) && (c.G() == 0) && (c.B() == 0));
}

Colour ray_colour(Ray& r, Intersect& intersect, Colour& bg, Light* light, SceneNode* root) {

  // dir vector needs to be normalized and backwards
  Vector3D v = -1 * r.m_dir; // IS THIS REALLY BACKWARDS SUPPOSED???
  v.normalize();

  Vector3D l = light->position - intersect.m_ipoint;
  l.normalize();

  Vector3D n = intersect.m_normal;

  PhongMaterial *mat = intersect.m_material;

  Colour px_colour(mat->m_kd);

  if (r.hit) {
    px_colour = px_colour * r.m_ambient; // plus ambient

    if (!colourIsZero(mat->m_kd)) {
      // check if shadow ray intersects with any other object in the scene
      px_colour = px_colour + ( mat->m_kd * direct_light(intersect, l, root) );
    }

    if (!colourIsZero(mat->m_ks)) {
      //r = ggReflection(r, intersect.m_normal);
      //px_colour = px_colour + (r.m_ks * ray_colour(r, intersect, px_colour, light));
      //cerr << "doing m_ks, pxcolour: " << px_colour << endl;
    }

    //cerr << "hit and px_colour: " << px_colour << endl;

    return px_colour;
  }

  return bg;
}
