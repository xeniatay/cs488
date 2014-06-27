#ifndef CS488_A4_HPP
#define CS488_A4_HPP

#include <string>
#include "algebra.hpp"
#include "scene.hpp"
#include "light.hpp"

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
               );

Ray ggReflection(Ray& r, Vector3D& N);
Colour ray_colour(Ray &r, Intersect& intersect, Colour& bg, Light* light, SceneNode* root);
Colour direct_light(Intersect& intersect, Vector3D& l, SceneNode* root);

bool colourIsZero(Colour& c);

#endif
