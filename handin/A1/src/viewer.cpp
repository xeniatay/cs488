#include "viewer.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>

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
  //Force a rerender
  Gtk::Allocation allocation = get_allocation();
  get_window()->invalidate_rect( allocation, false);

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

  // Just enable depth testing and set the background colour.
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.7, 0.7, 1.0, 0.0);

  gldrawable->gl_end();
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Clear the screen

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Modify the current projection matrix so that we move the
  // camera away from the origin.  We'll draw the game at the
  // origin, and we need to back up to see it.

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glTranslated(0.0, 0.0, -40.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Not implemented: set up lighting (if necessary)

  // Not implemented: scale and rotate the scene

  // You'll be drawing unit cubes, so the game will have width
  // 10 and height 24 (game = 20, stripe = 4).  Let's translate
  // the game so that we can draw it starting at (0,0) but have
  // it appear centered in the window.
  glTranslated(-5.0, -12.0, 0.0);

  // Not implemented: actually draw the current game state.
  // Here's some test code that draws red triangles at the
  // corners of the game board.
  glColor3d(1.0, 0.0, 0.0);

  glBegin(GL_TRIANGLES);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(1.0, 0.0, 0.0);
  glVertex3d(0.0, 1.0, 0.0);

  glVertex3d(9.0, 0.0, 0.0);
  glVertex3d(10.0, 0.0, 0.0);
  glVertex3d(10.0, 1.0, 0.0);

  glVertex3d(0.0, 19.0, 0.0);
  glVertex3d(1.0, 20.0, 0.0);
  glVertex3d(0.0, 20.0, 0.0);

  glVertex3d(10.0, 19.0, 0.0);
  glVertex3d(10.0, 20.0, 0.0);
  glVertex3d(9.0, 20.0, 0.0);
  glEnd();


  render_well(10, 20);

  // We pushed a matrix onto the PROJECTION stack earlier, we
  // need to pop it.

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

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

  // Set up perspective projection, using current size and aspect
  // ratio of display

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, event->width, event->height);
  gluPerspective(40.0, (GLfloat)event->width/(GLfloat)event->height, 0.1, 1000.0);

  // Reset to modelview matrix mode

  glMatrixMode(GL_MODELVIEW);

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

// Params: x, y z coords and r, g, b colour values
// Draws a unit cube (width = 1, height = 1, breadth = 1)
void Viewer::draw_cube(double x, double y, double z, double r = 0, double g = 0, double b = 0, double a = 1) {
  glBegin(GL_QUADS);
    glColor4d(r, g, b, a);
    glVertex3d (x, y, z); // bottom left (front)
    glVertex3d (x + 1, y, z); // bottom right (front)
    glVertex3d (x + 1, y + 1, z); // top right (front)
    glVertex3d (x, y + 1, z); // top left (front)

    glColor4d(r, g, b, a);
    glVertex3d (x, y, z + 1); // bottom left (back)
    glVertex3d (x + 1, y, z + 1); // bottom right (back)
    glVertex3d (x + 1, y + 1, z + 1); // top right (back)
    glVertex3d (x, y + 1, z + 1); // top left (back)

    glColor4d(r, g, b, a);
    glVertex3d (x, y, z); // bottom left (left)
    glVertex3d (x, y, z + 1); // bottom right (left)
    glVertex3d (x, y + 1, z + 1); // top right (left)
    glVertex3d (x, y + 1, z); // top left (left)

    glColor4d(r, g, b, a);
    glVertex3d (x, y, z); // bottom left (right)
    glVertex3d (x + 1, y, z + 1); // bottom right (right)
    glVertex3d (x + 1, y + 1, z + 1); // top right (right)
    glVertex3d (x + 1, y + 1, z); // top left (right)

    glColor4d(r, g, b, a);
    glVertex3d (x, y + 1, z); // bottom left (top)
    glVertex3d (x + 1, y + 1, z); // bottom right (top)
    glVertex3d (x + 1, y + 1, z + 1); // top right (top)
    glVertex3d (x, y + 1, z + 1); // top left (top)

    glColor4d(r, g, b, a);
    glVertex3d (x, y, z); // bottom left (bottom)
    glVertex3d (x + 1, y, z); // bottom right (bottom)
    glVertex3d (x + 1, y, z + 1); // bottom right (bottom)
    glVertex3d (x, y, z + 1); // bottom left (bottom)
  glEnd();
}

// Params: width, height
// Draws a U-shaped well of unit cubes
void Viewer::render_well(int width, int height) {
  int i = 0;

  for (i = -1; i < height; i++) {
    draw_cube(-1, i, 0, 0, 0, 1);
    draw_cube(width, i, 0, 0, 0, 1);
  }

  for (i = 0; i < width; i++) {
    draw_cube(i, -1, 0, 0, 0, 1);
  }
}