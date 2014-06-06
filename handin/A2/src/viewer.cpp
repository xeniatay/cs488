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

  std::cerr << "------- start set_perspective ---- " << std::endl;

  lbFar = projection(m_near, m_far, m_fov) * lb;
  ltFar = projection(m_near, m_far, m_fov) * lt;
  rbFar = projection(m_near, m_far, m_fov) * rb;
  rtFar = projection(m_near, m_far, m_fov) * rt;
  print_cube();

  lbFar = homogenizeProjection(lbFar, lb[2]);
  ltFar = homogenizeProjection(ltFar, lt[2]);
  rbFar = homogenizeProjection(rbFar, rb[2]);
  rtFar = homogenizeProjection(rtFar, rt[2]);

  print_cube();
  std::cerr << "------- end set_perspective ---- " << std::endl;

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

  // WIREFRAME BOX
  // ======================

  unset_origin();

  // Apply model coordinate matrix
  lt = m_mc_matrix * lt;
  lb = m_mc_matrix * lb;
  rt = m_mc_matrix * rt;
  rb = m_mc_matrix * rb;

  // Apply view coordinate matrix
  lt = m_vc_matrix * lt;
  lb = m_vc_matrix * lb;
  rt = m_vc_matrix * rt;
  rb = m_vc_matrix * rb;

  // MODEL COORDINATE GNOMON
  // ======================

  // Apply model coordinate matrix
  MCx = m_mc_coords_matrix * MCx;
  MCy = m_mc_coords_matrix * MCy;
  MCz = m_mc_coords_matrix * MCz;
  mc_origin = m_mc_coords_matrix * mc_origin;

  // Apply view coordinate matrix
  MCx = m_vc_matrix * MCx;
  MCy = m_vc_matrix * MCy;
  MCz = m_vc_matrix * MCz;
  mc_origin = m_vc_matrix * mc_origin;

  // WORLD COORDINATE GNOMON
  // ======================

  // Apply view coordinate matrix
  WCx = m_vc_matrix * WCx;
  WCy = m_vc_matrix * WCy;
  WCz = m_vc_matrix * WCz;
  origin = m_vc_matrix * origin;

  std::cerr << "---------- on_expose_event -----------" << std::endl;

  set_perspective(m_fov, m_near, m_far, m_aspect);

  set_origin();

  std::cerr << "---------- end on_expose_event -----------" << std::endl;


  draw_init(get_width(), get_height());
  set_colour(Colour(0.1, 0.1, 0.1));
  draw_2D_cube();

  set_colour(Colour(0.9, 0.9, 0.9));
  draw_model_axes();

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

  mouse_origin = event->x;

  on_expose_event(NULL);
  return true;
}

void Viewer::do_view_rotate() {

}

void Viewer::do_view_translate() {
  double displacement = 0.01 * m_axis_dir;

  if (m_axis == Viewer::XAXIS) {
    m_vc_matrix = translation( Vector3D(displacement * get_width(), 0, 0) );
  } else if (m_axis == Viewer::YAXIS) {
    m_vc_matrix = translation( Vector3D(0, displacement * get_height(), 0) );
  } else if (m_axis == Viewer::ZAXIS) {
    m_vc_matrix = translation( Vector3D(0, 0, displacement) );
  }
}

void Viewer::do_view_perspective() {

}

void Viewer::do_model_rotate() {
  std::cerr << "---- start model rotate ----" << std::endl;

  double angle = 10 * m_axis_dir;

  if (m_axis == Viewer::XAXIS) {
    m_mc_matrix = rotation(angle, 'x');
  } else if (m_axis == Viewer::YAXIS) {
    m_mc_matrix = rotation(angle, 'y');
  } else if (m_axis == Viewer::ZAXIS) {
    m_mc_matrix = rotation(angle, 'z');
  }

  m_mc_coords_matrix = m_mc_matrix;

  std::cerr << "---- end model rotate ----" << std::endl;
}

void Viewer::do_model_translate() {
  std::cerr << "---- start model translate ----" << std::endl;

  double displacement = 0.01 * m_axis_dir;

  if (m_axis == Viewer::XAXIS) {
    m_mc_matrix = translation( Vector3D(displacement * get_width(), 0, 0) );
  } else if (m_axis == Viewer::YAXIS) {
    m_mc_matrix = translation( Vector3D(0, displacement * get_height(), 0) );
  } else if (m_axis == Viewer::ZAXIS) {
    m_mc_matrix = translation( Vector3D(0, 0, displacement) );
  }

  m_mc_coords_matrix = m_mc_matrix;
  print_mc();
  print_cube();

  std::cerr << "---- end model translate ----" << std::endl;
}

void Viewer::do_model_scale() {
  double scaleFactor = (m_axis_dir > 0) ? 1.05 : 0.95;

  if (m_axis == Viewer::XAXIS) {
    m_mc_matrix = scaling( Vector3D(scaleFactor, 1, 1) );
  } else if (m_axis == Viewer::YAXIS) {
    m_mc_matrix = scaling( Vector3D(1, scaleFactor, 1) );
  } else if (m_axis == Viewer::ZAXIS) {
    // TODO idk
    m_mc_matrix = scaling( Vector3D(1, 1, scaleFactor) );
  }
}

void Viewer::do_viewport() {

}

void Viewer::reset() {
  mouse_origin = 0;
  m_mc_matrix = Matrix4x4();
  m_mc_coords_matrix = Matrix4x4();
  m_vc_matrix = Matrix4x4();

  m_near = 2;
  m_far = m_near + 8;
  m_fov = 90;
  m_aspect = 2;

  MCx = Point3D(0.25 * get_width(), 0, 1);
  MCy = Point3D(0, 0.25 * get_height(), 1);
  MCy = reflection('x') * MCy;
  MCz = Point3D(0, 0, 1);

  WCx = Point3D(0.25 * get_width(), 0, 1);
  WCy = Point3D(0, 0.25 * get_height(), 1);
  WCy = reflection('x') * WCy;
  WCz = Point3D(0, 0, 1);

  // ORIGIN HELPERS
  // ======================
  origin = Vector3D(0.5 * get_width(), 0.5 * get_height(), 1);
  mc_origin = origin;
  displaceToOrigin = Vector3D(0.5 * get_width(), 0.5 * get_height(), 0);
  displaceFromOrigin = Vector3D(-0.5 * get_width(), -0.5 * get_height(), 0);
  translateToOrigin = translation(displaceToOrigin);
  translateFromOrigin = translation(displaceFromOrigin);

  // BUILD WIREFRAME BOX
  // =====================
  // NEAR PLANE
  lt = Point3D(-0.4 * get_width(), -0.4 * get_height(), m_near);
  lb = Point3D(-0.4 * get_width(), 0.4 * get_height(), m_near);
  rt = Point3D(0.4 * get_width(), -0.4 * get_height(), m_near);
  rb = Point3D(0.4 * get_width(), 0.4 * get_height(), m_near);

  // FAR PLANE
  scaleToFarPlane = Vector3D(0.7, 0.7, 0);
  ltFar = scaling(scaleToFarPlane) * lt;
  lbFar = scaling(scaleToFarPlane) * lb;
  rtFar = scaling(scaleToFarPlane) * rt;
  rbFar = scaling(scaleToFarPlane) * rb;

  set_mode(MODEL_ROTATE);

  set_origin();
  on_expose_event(NULL);
}

void Viewer::print_cube() {
  std::cerr << "---- start print cube ----" << std::endl;
  std::cerr << "lt: " << lt << std::endl;
  std::cerr << "lb: " << lb << std::endl;
  std::cerr << "rt: " << rt << std::endl;
  std::cerr << "rb: " << rb << std::endl;
  std::cerr << "ltFar: " << ltFar << std::endl;
  std::cerr << "lbFar: " << lbFar << std::endl;
  std::cerr << "rtFar: " << rtFar << std::endl;
  std::cerr << "rbFar: " << rbFar << std::endl;
  std::cerr << "---- end print cube ----" << std::endl;
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

void Viewer::set_origin() {
  lt = translateToOrigin * lt;
  lb = translateToOrigin * lb;
  rt = translateToOrigin * rt;
  rb = translateToOrigin * rb;
  ltFar = translateToOrigin * ltFar;
  lbFar = translateToOrigin * lbFar;
  rtFar = translateToOrigin * rtFar;
  rbFar = translateToOrigin * rbFar;

  WCx = translateToOrigin * WCx;
  WCy = translateToOrigin * WCy;
  WCz = translateToOrigin * WCz;

  MCx = translateToOrigin * MCx;
  MCy = translateToOrigin * MCy;
  MCz = translateToOrigin * MCz;
}

void Viewer::unset_origin() {
  // TRANSLATE ALL PLANES
  lt = translateFromOrigin * lt;
  lb = translateFromOrigin * lb;
  rt = translateFromOrigin * rt;
  rb = translateFromOrigin * rb;
  ltFar = translateFromOrigin * ltFar;
  lbFar = translateFromOrigin * lbFar;
  rtFar = translateFromOrigin * rtFar;
  rbFar = translateFromOrigin * rbFar;

  WCx = translateFromOrigin * WCx;
  WCy = translateFromOrigin * WCy;
  WCz = translateFromOrigin * WCz;

  MCx = translateFromOrigin * MCx;
  MCy = translateFromOrigin * MCy;
  MCz = translateFromOrigin * MCz;
}

void Viewer::draw_2D_cube() {
  /// draw near plane
  draw_line(Point2D(lbv[0], lbv[1]), Point2D(lt[0], lt[1])); // left vertical
  draw_line(Point2D(rb[0], rb[1]), Point2D(rt[0], rt[1])); // right vertical
  draw_line(Point2D(lt[0], lt[1]), Point2D(rt[0], rt[1])); // top horiz
  draw_line(Point2D(lb[0], lb[1]), Point2D(rb[0], rb[1])); // bottom horiz
  // draw far plane
  draw_line(Point2D(lbFar[0], lbFar[1]), Point2D(ltFar[0], ltFar[1])); // left vertical
  draw_line(Point2D(rbFar[0], rbFar[1]), Point2D(rtFar[0], rtFar[1])); // right vertical
  draw_line(Point2D(ltFar[0], ltFar[1]), Point2D(rtFar[0], rtFar[1])); // top horiz
  draw_line(Point2D(lbFar[0], lbFar[1]), Point2D(rbFar[0], rbFar[1])); // bottom horiz
  /*
  // draw left plane
  draw_line(Point2D(lbFar[0], lbFar[1]), Point2D(ltFar[0], ltFar[1])); // left vertical
  draw_line(Point2D(lb[0], lb[1]), Point2D(lt[0], lt[1])); // right vertical
  draw_line(Point2D(ltFar[0], ltFar[1]), Point2D(lt[0], lt[1])); // top horiz
  draw_line(Point2D(lbFar[0], lbFar[1]), Point2D(lb[0], lb[1])); // bottom horiz
  // draw right plane
  draw_line(Point2D(rbFar[0], rbFar[1]), Point2D(rtFar[0], rtFar[1])); // left vertical
  draw_line(Point2D(rb[0], rb[1]), Point2D(rt[0], rt[1])); // right vertical
  draw_line(Point2D(rtFar[0], rtFar[1]), Point2D(rt[0], rt[1])); // top horiz
  draw_line(Point2D(rbFar[0], rbFar[1]), Point2D(rb[0], rb[1])); // bottom horiz
  */
}

void Viewer::draw_world_axes() {
  draw_line(Point2D(origin[0], origin[1]), Point2D(WCx[0], WCx[1]));
  draw_line(Point2D(origin[0], origin[1]), Point2D(WCy[0], WCy[1]));
  draw_line(Point2D(origin[0], origin[1]), Point2D(WCz[0], WCz[1]));
}

void Viewer::draw_model_axes() {
  draw_line(Point2D(mc_origin[0], mc_origin[1]), Point2D(MCx[0], MCx[1]));
  draw_line(Point2D(mc_origin[0], mc_origin[1]), Point2D(MCy[0], MCy[1]));
  draw_line(Point2D(mc_origin[0], mc_origin[1]), Point2D(MCz[0], MCz[1]));
}