#include "viewer.hpp"
#include "algebra.hpp"
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "trackball.hpp"
#include "events.hpp"
#include "material.hpp"

using std::cerr;
using std::endl;

typedef std::list<SceneNode*> SN;
extern SN all_scenenodes;

typedef std::list<GeometryNode*> GN;
extern GN all_geonodes;

typedef GLdouble Matrix[4][4];

Viewer::Viewer() {
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

  glShadeModel(GL_SMOOTH);
  glClearColor( 0.4, 0.4, 0.4, 0.0 );
  glEnable(GL_DEPTH_TEST);

  gldrawable->gl_end();

  cerr << "On realize!" << endl;
  reset_all();

  // default mode
  m_mode = POSITION_OR_ORIENTATION;
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  cerr << "On expose event!" << endl;
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Set up for perspective drawing
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, get_width(), get_height());
  gluPerspective(40.0, (GLfloat)get_width()/(GLfloat)get_height(), 0.1, 1000.0);

  // change to model view for drawing
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Clear framebuffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Trackball stuff
  Matrix *mRot = getMRot();
  Matrix *mTrans = getMTrans();
  //glPushMatrix();
  glLoadMatrixd((GLdouble *) mTrans);
  glMultMatrixd((GLdouble *) mRot);

  // Set up lighting

  // Draw stuff
  m_scenenode->walk_gl();

  // TODO make this optional
  draw_trackball_circle();

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

  x_origin = event->x;
  y_origin = event->y;

  if (m_mode == POSITION_OR_ORIENTATION) {
    switch (event->button) {
      case 1:
        // B1
        vToggleDir(DIR_X);
        break;
      case 2:
        vToggleDir(DIR_Y);
        // B2
        break;
      case 3:
        vToggleDir(DIR_Z);
        // B3
        break;
      default:
        break;
    }
  } else {
    // picking doesn't work
    // picking();
  }

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

  m_axis_dir = ( (event->x - x_origin) < 0) ? -1 : 1;

  if (m_mode == POSITION_OR_ORIENTATION) {

    float x_current = event->x,
          y_current = event->y;

    vPerformTransfo((float)x_origin, x_current, (float)y_origin, y_current);

    x_origin = x_current;
    y_origin = y_current;


  } else {
    // joint

    int j = 0;

    for( std::list<GeometryNode*>::const_iterator i = all_geonodes.begin(); i != all_geonodes.end(); ++i ) {

      GeometryNode* node = (*i);

      for (j = 0; j < numLimbs; j++) {
        if (node->m_geo_id == j) {
          node->rotate('x', 1 * m_axis_dir);
        }
      }
    }

  }

  invalidate();

  return true;
}

void Viewer::draw_trackball_circle()
{
  int current_width = get_width();
  int current_height = get_height();

  // Set up for orthogonal drawing to draw a circle on screen.
  // You'll want to make the rest of the function conditional on
  // whether or not we want to draw the circle this time around.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, current_width, current_height);
  glOrtho(0.0, (float)current_width,
           0.0, (float)current_height, -0.1, 0.1);

  // change to model view for drawing
  glMatrixMode(GL_MODELVIEW);

  // Reset modelview matrix
  glLoadIdentity();

  // draw a circle for use with the trackball
  glDisable(GL_LIGHTING);
  glEnable(GL_LINE_SMOOTH);
  glColor3f(1.0, 1.0, 1.0);
  double radius = current_width < current_height ?
    (float)current_width * 0.25 : (float)current_height * 0.25;
  glTranslated((float)current_width * 0.5, (float)current_height * 0.5, 0);
  glBegin(GL_LINE_LOOP);
  for(size_t i=0; i<40; ++i) {
    double cosine = radius * cos(i*2*M_PI/40);
    double sine = radius * sin(i*2*M_PI/40);
    glVertex2f(cosine, sine);
  }
  glEnd();
  glColor3f(0.0, 0.0, 0.0);
  glDisable(GL_LINE_SMOOTH);
}

void Viewer::reset_position() {

}

void Viewer::reset_orientation() {

}

void Viewer::reset_joints() {

}

void Viewer::reset_all() {
  m_axis_dir = 1;
  x_origin = 0;
  y_origin = 0;
  m_width = get_width();
  m_height = get_height();
  numLimbs = 18;
  picked_list.resize(numLimbs);
  std::fill(picked_list.begin(), picked_list.end(), false);

  m_scenenode = all_scenenodes.front();
  m_geonode = all_geonodes.front();
  cerr << "M_SCENENODE: " << m_scenenode->m_name << endl;


  resetMRot();
  resetMTrans();

  vToggleDir(DIR_NONE);

  invalidate();
}

void Viewer::undo() {

}

void Viewer::redo() {

}

void Viewer::set_options(Option option) {

}

void Viewer::picking_in_select_mode() {
  int size = 100, hits;

  GLuint buffer[size];
  GLint viewport[4];

  glSelectBuffer(size, buffer);

  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(-1);

  glGetIntegerv(GL_VIEWPORT, viewport);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPickMatrix(x_origin, y_origin, 10, 10, viewport);

  glMatrixMode(GL_MODELVIEW);

  for( std::list<SceneNode*>::const_iterator i = all_scenenodes.begin(); i != all_scenenodes.end(); ++i ) {
    SceneNode *node = (*i);

    glLoadName(node->m_id);
    node->walk_gl();
  }

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  hits = glRenderMode(GL_RENDER);
  cerr << "HITS: " << hits << endl;
}

void Viewer::set_pickings(Picking picked) {

  if (picked_list.at(picked) == true) {
    picked_list.at(picked) = false;
  } else {

    for( std::list<GeometryNode*>::const_iterator i = all_geonodes.begin(); i != all_geonodes.end(); ++i ) {
      GeometryNode* node = (*i);
      if (node->m_geo_id == picked) {
        PhongMaterial *material = new PhongMaterial(Colour(1.0, 1.0, 0.0),
                                             Colour(0.1, 0.1, 0.1),
                                             10);
        node->set_material(material);
      }
    }

    picked_list.at(picked) = true;
  }

  invalidate();
}