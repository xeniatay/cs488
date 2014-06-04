// Xenia Tay
// 20396769
// xzytay

#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "algebra.hpp"

// The "main" OpenGL widget
class Viewer : public Gtk::GL::DrawingArea {
public:
  Viewer();
  virtual ~Viewer();

  enum Mode {
    VIEW_ROTATE,
    VIEW_TRANSLATE,
    VIEW_PERSPECTIVE,
    MODEL_ROTATE,
    MODEL_TRANSLATE,
    MODEL_SCALE,
    VIEWPORT
  };

  enum Axis {
    XAXIS,
    YAXIS,
    ZAXIS
  };

  // A useful function that forces this widget to rerender. If you
  // want to render a new frame, do not call on_expose_event
  // directly. Instead call this, which will cause an on_expose_event
  // call when the time is right.
  void invalidate();

  // *** Fill in these functions (in viewer.cpp) ***

  // Set the parameters of the current perspective projection using
  // the semantics of gluPerspective().
  void set_perspective(double fov, double aspect,
                       double near, double far);

  // Restore all the transforms and perspective parameters to their
  // original state. Set the viewport to its initial size.
  void reset_view();

  void set_mode(Mode mode) { m_mode = mode; }
  void reset();

  void print_cube();
  void print_vc();
  void print_mc();
  void print_axes();

  void draw_2D_cube();
  void draw_model_axes();
  void draw_world_axes();

  void do_view_rotate();
  void do_view_translate();
  void do_view_perspective();
  void do_model_rotate();
  void do_model_translate();
  void do_model_scale();
  void do_viewport();

protected:

  // Events we implement
  // Note that we could use gtkmm's "signals and slots" mechanism
  // instead, but for many classes there's a convenient member
  // function one just needs to define that'll be called with the
  // event.

  // Called when GL is first initialized
  virtual void on_realize();
  // Called when our window needs to be redrawn
  virtual bool on_expose_event(GdkEventExpose* event);
  // Called when the window is resized
  virtual bool on_configure_event(GdkEventConfigure* event);
  // Called when a mouse button is pressed
  virtual bool on_button_press_event(GdkEventButton* event);
  // Called when a mouse button is released
  virtual bool on_button_release_event(GdkEventButton* event);
  // Called when the mouse moves
  virtual bool on_motion_notify_event(GdkEventMotion* event);

private:

  // *** Fill me in ***
  // You will want to declare some more matrices here
  Matrix4x4 m_projection;
  Matrix4x4 m_mc_matrix;
  Matrix4x4 m_vc_matrix;

  Vector3D MCx;
  Vector3D MCy;
  Vector3D MCz;

  Vector3D WCx;
  Vector3D WCy;
  Vector3D WCz;

  Mode m_mode;
  Axis m_axis;
  int m_axis_dir;
  int mouse_origin;

  Vector3D origin;
  Vector3D mc_origin;
  Vector3D displaceToOrigin;
  Matrix4x4 translateToOrigin;

  Vector3D lt;
  Vector3D lb;
  Vector3D rt;
  Vector3D rb;

  // FAR PLANE
  Vector3D scaleToFarPlane;
  Vector3D ltFar;
  Vector3D lbFar;
  Vector3D rtFar;
  Vector3D rbFar;
};

#endif
