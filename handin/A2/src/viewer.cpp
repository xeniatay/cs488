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

  reset();
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
  // ======================
  set_colour(Colour(0.1, 0.1, 0.1));

  // Apply modelGnomon matrix
  lt = m_mc_matrix * lt;
  lb = m_mc_matrix * lb;
  rt = m_mc_matrix * rt;
  rb = m_mc_matrix * rb;
  ltFar = m_mc_matrix * ltFar;
  lbFar = m_mc_matrix * lbFar;
  rtFar = m_mc_matrix * rtFar;
  rbFar = m_mc_matrix * rbFar;

  // Apply viewGnomon matrix
  lt = m_vc_matrix * lt;
  lb = m_vc_matrix * lb;
  rt = m_vc_matrix * rt;
  rb = m_vc_matrix * rb;
  ltFar = m_vc_matrix * ltFar;
  lbFar = m_vc_matrix * lbFar;
  rtFar = m_vc_matrix * rtFar;
  rbFar = m_vc_matrix * rbFar;

  draw_2D_cube();

  // MODEL COORDINATE GNOMON
  // ======================
  set_colour(Colour(0.9, 0.9, 0.9));

  // Apply modelGnomon matrix
  MCx = m_mc_matrix * MCx;
  MCy = m_mc_matrix * MCy;
  MCz = m_mc_matrix * MCz;
  mc_origin = m_mc_matrix * mc_origin;

  // Apply viewGnomon matrix
  MCx = m_vc_matrix * MCx;
  MCy = m_vc_matrix * MCy;
  MCz = m_vc_matrix * MCz;
  mc_origin = m_vc_matrix * mc_origin;

  draw_model_axes();

  // WORLD COORDINATE GNOMON
  // ======================
  set_colour(Colour(0.9, 0.0, 0.0));

  draw_world_axes();
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

  // get axis
  switch (event->button) {
    case 1:
      m_axis = Viewer::XAXIS;
      break;
    case 2:
      m_axis = Viewer::YAXIS;
      break;
    case 3:
      m_axis = Viewer::ZAXIS;
      break;
    default:
      break;
  }

  // shift button pressed
  if (event->state & GDK_SHIFT_MASK) {
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

  m_axis_dir = ( (event->x - mouse_origin) < 0) ? -1 : 1;

  /*
  switch (m_mode) {
    case VIEW_ROTATE:
      do_view_rotate();
      break;
    case VIEW_TRANSLATE:
      do_view_translate();
      break;
    case VIEW_PERSPECTIVE:
      do_view_perspective();
      break;
    case MODEL_ROTATE:
      do_model_rotate();
      break;
    case MODEL_TRANSLATE:
      do_model_translate();
      break;
    case MODEL_SCALE:
      do_model_scale();
      break;
    case VIEWPORT:
      do_viewport();
      break;
    default:
      break;
  }
  */
  do_model_translate();

  mouse_origin = event->x;

  on_expose_event(NULL);
  return true;
}

void Viewer::do_view_rotate() {

}

void Viewer::do_view_translate() {

}

void Viewer::do_view_perspective() {

}

void Viewer::do_model_rotate() {
  if (m_axis == Viewer::XAXIS) {
    //m_vc_matrix = m_vc_matrix * rotation(30 * m_axis_dir, 'x');
  } else if (m_axis == Viewer::YAXIS) {
    //m_mc_matrix = m_mc_matrix * rotation(30 * m_axis_dir, 'y');
  } else if (m_axis == Viewer::ZAXIS) {
    //m_mc_matrix = m_mc_matrix * rotation(30 * m_axis_dir, 'z');
  }
}

void Viewer::do_model_translate() {
  double displacement = 0.01 * m_axis_dir;

  if (m_axis == Viewer::XAXIS) {
    m_mc_matrix = translation( Vector3D(displacement * get_width(), 0, 0) );
  } else if (m_axis == Viewer::YAXIS) {
    m_mc_matrix = translation( Vector3D(0, displacement * get_height(), 0) );
  } else if (m_axis == Viewer::ZAXIS) {
    // TODO the axes here are broken
    Vector3D scaleFactor = (m_axis_dir > 0) ? Vector3D(1.05, 1.05, 1) : Vector3D(0.95, 0.95, 1);
    m_mc_matrix = scaling(scaleFactor);
  }

  print_mc();
  print_axes();
}

void Viewer::do_model_scale() {

}

void Viewer::do_viewport() {

}

void Viewer::reset() {
  mouse_origin = 0;
  m_mc_matrix = Matrix4x4();
  m_vc_matrix = Matrix4x4();

  MCx = Vector3D(0.25 * get_width(), 0, 1);
  MCy = Vector3D(0, 0.25 * get_height(), 1);
  MCy = reflection('x') * MCy;
  MCz = Vector3D(0, 0, 1);

  WCx = Vector3D(0.25 * get_width(), 0, 1);
  WCy = Vector3D(0, 0.25 * get_height(), 1);
  WCy = reflection('x') * WCy;
  WCz = Vector3D(0, 0, 1);

  // ORIGIN HELPERS
  // ======================
  origin = Vector3D(0.5 * get_width(), 0.5 * get_height(), 1);
  mc_origin = origin;
  displaceToOrigin = Vector3D(0.5 * get_width(), 0.5 * get_height(), 0);
  translateToOrigin = translation(displaceToOrigin);

  // BUILD WIREFRAME BOX
  // =====================
  // NEAR PLANE
  lt = Vector3D(-0.4 * get_width(), -0.4 * get_height(), 1);
  lb = Vector3D(-0.4 * get_width(), 0.4 * get_height(), 1);
  rt = Vector3D(0.4 * get_width(), -0.4 * get_height(), 1);
  rb = Vector3D(0.4 * get_width(), 0.4 * get_height(), 1);

  // FAR PLANE
  scaleToFarPlane = Vector3D(0.7, 0.7, 0);
  ltFar = scaling(scaleToFarPlane) * lt;
  lbFar = scaling(scaleToFarPlane) * lb;
  rtFar = scaling(scaleToFarPlane) * rt;
  rbFar = scaling(scaleToFarPlane) * rb;
}

void Viewer::print_cube() {
  std::cerr << "lt: " << lt << std::endl;
  std::cerr << "lb: " << lb << std::endl;
  std::cerr << "rt: " << rt << std::endl;
  std::cerr << "rb: " << rb << std::endl;
}

void Viewer::print_mc() {
  std::cerr << "MC Matrix: " << std::endl << m_mc_matrix << std::endl;
}

void Viewer::print_vc() {
  std::cerr << "VC Matrix: " << std::endl << m_vc_matrix << std::endl;
}

void Viewer::print_axes() {
  std::cerr << "World Origin: " << origin << std::endl;
  std::cerr << "WCx: " << WCx << std::endl;
  std::cerr << "WCy: " << WCy << std::endl;
  std::cerr << "WCz: " << WCz << std::endl;
  std::cerr << "MC Origin: " << mc_origin << std::endl;
  std::cerr << "MCx: " << MCx << std::endl;
  std::cerr << "MCy: " << MCy << std::endl;
  std::cerr << "MCz: " << MCz << std::endl;
}

void Viewer::draw_2D_cube() {
  // TRANSLATE ALL PLANES
  Vector3D lt2D = translateToOrigin * lt;
  Vector3D lb2D = translateToOrigin * lb;
  Vector3D rt2D = translateToOrigin * rt;
  Vector3D rb2D = translateToOrigin * rb;
  Vector3D lt2DFar = translateToOrigin * ltFar;
  Vector3D lb2DFar = translateToOrigin * lbFar;
  Vector3D rt2DFar = translateToOrigin * rtFar;
  Vector3D rb2DFar = translateToOrigin * rbFar;

  /// draw near plane
  draw_line(Point2D(lb2D[0], lb2D[1]), Point2D(lt2D[0], lt2D[1])); // left vert2Dical
  draw_line(Point2D(rb2D[0], rb2D[1]), Point2D(rt2D[0], rt2D[1])); // right vert2Dical
  draw_line(Point2D(lt2D[0], lt2D[1]), Point2D(rt2D[0], rt2D[1])); // top horiz
  draw_line(Point2D(lb2D[0], lb2D[1]), Point2D(rb2D[0], rb2D[1])); // bottom horiz
  // draw far plane
  draw_line(Point2D(lb2DFar[0], lb2DFar[1]), Point2D(lt2DFar[0], lt2DFar[1])); // left vert2Dical
  draw_line(Point2D(rb2DFar[0], rb2DFar[1]), Point2D(rt2DFar[0], rt2DFar[1])); // right vert2Dical
  draw_line(Point2D(lt2DFar[0], lt2DFar[1]), Point2D(rt2DFar[0], rt2DFar[1])); // top horiz
  draw_line(Point2D(lb2DFar[0], lb2DFar[1]), Point2D(rb2DFar[0], rb2DFar[1])); // bottom horiz
  // draw left plane
  draw_line(Point2D(lb2DFar[0], lb2DFar[1]), Point2D(lt2DFar[0], lt2DFar[1])); // left vert2Dical
  draw_line(Point2D(lb2D[0], lb2D[1]), Point2D(lt2D[0], lt2D[1])); // right vert2Dical
  draw_line(Point2D(lt2DFar[0], lt2DFar[1]), Point2D(lt2D[0], lt2D[1])); // top horiz
  draw_line(Point2D(lb2DFar[0], lb2DFar[1]), Point2D(lb2D[0], lb2D[1])); // bottom horiz
  // draw right plane
  draw_line(Point2D(rb2DFar[0], rb2DFar[1]), Point2D(rt2DFar[0], rt2DFar[1])); // left vert2Dical
  draw_line(Point2D(rb2D[0], rb2D[1]), Point2D(rt2D[0], rt2D[1])); // right vert2Dical
  draw_line(Point2D(rt2DFar[0], rt2DFar[1]), Point2D(rt2D[0], rt2D[1])); // top horiz
  draw_line(Point2D(rb2DFar[0], rb2DFar[1]), Point2D(rb2D[0], rb2D[1])); // bottom horiz
}

void Viewer::draw_world_axes() {
  // Translate to origin
  Vector3D WC2Dx = translateToOrigin * WCx;
  Vector3D WC2Dy = translateToOrigin * WCy;
  Vector3D WC2Dz = translateToOrigin * WCz;

  draw_line(Point2D(origin[0], origin[1]), Point2D(WC2Dx[0], WC2Dx[1]));
  draw_line(Point2D(origin[0], origin[1]), Point2D(WC2Dy[0], WC2Dy[1]));
  draw_line(Point2D(origin[0], origin[1]), Point2D(WC2Dz[0], WC2Dz[1]));
}

void Viewer::draw_model_axes() {
  // Translate to Origin
  Vector3D MC2Dx = translateToOrigin * MCx;
  Vector3D MC2Dy = translateToOrigin * MCy;
  Vector3D MC2Dz = translateToOrigin * MCz;

  draw_line(Point2D(mc_origin[0], mc_origin[1]), Point2D(MC2Dx[0], MC2Dx[1]));
  draw_line(Point2D(mc_origin[0], mc_origin[1]), Point2D(MC2Dy[0], MC2Dy[1]));
  draw_line(Point2D(mc_origin[0], mc_origin[1]), Point2D(MC2Dz[0], MC2Dz[1]));
}