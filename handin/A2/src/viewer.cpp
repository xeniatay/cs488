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

  // origin
  Point2D origin = Point2D(0.5 * get_width(), 0.5 * get_height());
  Vector3D displaceToOrigin = Vector3D(0.5 * get_width(), 0.5 * get_height(), 0);
  Matrix4x4 translateToOrigin = translation(displaceToOrigin);

  // MODEL COORDINATE GNOMON
  // ======================
  set_colour(Colour(0.9, 0.9, 0.9));

  Matrix4x4 modelGnomon = rotation(90, 'y');

  Vector3D MCx = Vector3D(0.25 * get_width(), 0, 1);
  Vector3D MCy = Vector3D(0, 0.25 * get_height(), 1);
  Vector3D MCz = Vector3D(0, 0, 1);

  /* is z axis just 0, 0, 0???
  // z = x rotated by 90deg about y axis
  Vector3D MCz = modelGnomon * MCx;

  // adjust values of MCz
  MCz[0] = fabs(MCz[0]);
  MCz[1] = fabs(MCz[1]);
  MCz[2] = 1;
  */

  std::cerr << "Origin: " << origin << std::endl;
  //std::cerr << "MCz after rotate: " << MCz << std::endl;

  modelGnomon = reflection('x');
  MCy = modelGnomon * MCy;

  //std::cerr << "MCy before reflection: " << MCy << std::endl;
  //std::cerr << "MCy after reflection: " << MCy << std::endl;

  //std::cerr << "MCx before translate: " << MCx << std::endl;
  //std::cerr << "MCy before translate: " << MCy << std::endl;
  //std::cerr << "MCz before translate: " << MCz << std::endl;

  MCx = translateToOrigin * MCx;
  MCy = translateToOrigin * MCy;
  MCz = translateToOrigin * MCz;

  std::cerr << "MCx after translate: " << MCx << std::endl;
  std::cerr << "MCy after translate: " << MCy << std::endl;
  std::cerr << "MCz after translate: " << MCz << std::endl;

  draw_line(origin, Point2D(MCx[0], MCx[1]));
  draw_line(origin, Point2D(MCy[0], MCy[1]));
  draw_line(origin, Point2D(MCz[0], MCz[1]));

  // WORLD COORDINATE GNOMON
  // ======================
  set_colour(Colour(0.9, 0.0, 0.0));

  Vector3D WCx = Vector3D(0.25 * get_width(), 0, 1);
  Vector3D WCy = Vector3D(0, 0.25 * get_height(), 1);

  Matrix4x4 worldGnomon = reflection('x');
  WCy = worldGnomon * WCy;

  WCx = translateToOrigin * WCx;
  WCy = translateToOrigin * WCy;

  draw_line(origin, Point2D(WCx[0], WCx[1]));
  draw_line(origin, Point2D(WCy[0], WCy[1]));
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
