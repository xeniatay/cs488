// Xenia Tay
// 20396769
// xzytay

#include "viewer.hpp"
#include "a2.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include "draw.hpp"
#include <math.h>

Viewer::Viewer()
{
  Glib::RefPtr<Gdk::GL::Config> glconfig;

  // Ask for an OpenGL Setup with
  //  - red, green and blue component colour
  //  - a depth buffer to avoid things overlapping wrongly
  //  - double-buffered rendering to avoid tearing/flickering
  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB |
                                     Gdk::GL::MODE_DEPTH |
                                     Gdk::GL::MODE_DOUBLE);
  if (glconfig == 0) {
    // If we can't get this configuration, die
    std::cerr << "Unable to setup OpenGL Configuration!" << std::endl;
    abort();
  }

  // Accept the configuration
  set_gl_capability(glconfig);

  // Register the fact that we want to receive these events
  add_events(Gdk::BUTTON1_MOTION_MASK    |
             Gdk::BUTTON2_MOTION_MASK    |
             Gdk::BUTTON3_MOTION_MASK    |
             Gdk::BUTTON_PRESS_MASK      |
             Gdk::BUTTON_RELEASE_MASK    |
             Gdk::VISIBILITY_NOTIFY_MASK);
}

Viewer::~Viewer()
{
  // Nothing to do here right now.
}

void Viewer::invalidate()
{
  // Force a rerender
  Gtk::Allocation allocation = get_allocation();
  get_window()->invalidate_rect( allocation, false);
}

void Viewer::set_perspective(double fov, double aspect,
                             double near, double far)
{
  // Fill me in!
}

void Viewer::reset_view()
{
  // Fill me in!
}

void Viewer::on_realize()
{
  // Do some OpenGL setup.
  // First, let the base class do whatever it needs to
  Gtk::GL::DrawingArea::on_realize();

  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable)
    return;

  if (!gldrawable->gl_begin(get_gl_context()))
    return;

  gldrawable->gl_end();
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Here is where your drawing code should go.

  draw_init(get_width(), get_height());

  // WIREFRAME BOX
  set_colour(Colour(0.1, 0.1, 0.1));

  // left vertical
  draw_line(Point2D(0.1*get_width(), 0.1*get_height()),
            Point2D(0.1*get_width(), 0.9*get_height()));
  // right vertical
  draw_line(Point2D(0.9*get_width(), 0.1*get_height()),
            Point2D(0.9*get_width(), 0.9*get_height()));
  // top horiz
  draw_line(Point2D(0.1*get_width(), 0.1*get_height()),
            Point2D(0.9*get_width(), 0.1*get_height()));
  // bottom horiz
  draw_line(Point2D(0.1*get_width(), 0.9*get_height()),
            Point2D(0.9*get_width(), 0.9*get_height()));

  // GNOMON
  set_colour(Colour(0.9, 0.9, 0.9));

  Point2D origin = Point2D(0.5 * get_width(), 0.5 * get_height());

  Matrix4x4 modelGnomon = rotation(90, 'z');

  // cross x and y to get z axis
  Vector3D xaxis = Vector3D(0.25 * get_width(), 0, 1);
  Vector3D yaxis = Vector3D(0, 0.25 * get_height(), 1);
  Vector3D zaxis = modelGnomon * xaxis;
  zaxis[0] = fabs(zaxis[0]);
  zaxis[1] = fabs(zaxis[1]);

  std::cerr << "Origin: " << origin << std::endl;

  std::cerr << "zaxis after rotate: " << zaxis << std::endl;

/*
  Vector3D zaxisNorm = zaxis;
  zaxisNorm.normalize();

  std::cerr << "zaxisNorm: " << zaxisNorm << std::endl;

  zaxisNorm[0] = fabs(zaxisNorm[0]);
  zaxisNorm[1] = fabs(zaxisNorm[1]);
  zaxisNorm[2] = fabs(zaxisNorm[2]);
  std::cerr << zaxis.length() << std::endl;

  std::cerr << "zaxisNorm after fabs: " << zaxisNorm << std::endl;
*/

  modelGnomon = reflection('x');

  std::cerr << "yaxis before reflection: " << yaxis << std::endl;
  yaxis = modelGnomon * yaxis;

  std::cerr << "yaxis after reflection: " << yaxis << std::endl;

  Vector3D displaceToOrigin = Vector3D(0.5 * get_width(), 0.5 * get_height(), 0);
  modelGnomon = translation(displaceToOrigin);
  std::cerr << "modelGnomon from translate: " << modelGnomon << std::endl;


  std::cerr << "xaxis before translate: " << xaxis << std::endl;
  xaxis = modelGnomon * xaxis;
  yaxis = modelGnomon * yaxis;
  zaxis = modelGnomon * zaxis;

  std::cerr << "xaxis after translate: " << xaxis << std::endl;
  std::cerr << "yaxis after translate: " << yaxis << std::endl;
  std::cerr << "zaxis after translate: " << zaxis << std::endl;

  // x-axis
  draw_line(origin, Point2D(xaxis[0], xaxis[1]));
  // y-axis
  draw_line(origin, Point2D(yaxis[0], yaxis[1]));
  // z-axis
  draw_line(origin, Point2D(zaxis[0], zaxis[1]));

  draw_complete();

  // Swap the contents of the front and back buffers so we see what we
  // just drew. This should only be done if double buffering is enabled.
  gldrawable->swap_buffers();

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_configure_event(GdkEventConfigure* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_button_press_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " pressed" << std::endl;
  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  std::cerr << "Stub: Button " << event->button << " released" << std::endl;
  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  std::cerr << "Stub: Motion at " << event->x << ", " << event->y << std::endl;
  return true;
}
