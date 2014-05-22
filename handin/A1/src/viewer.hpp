#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "game.hpp"

// The "main" OpenGL widget
class Viewer : public Gtk::GL::DrawingArea {
public:
  Viewer();
  virtual ~Viewer();

  // drawing modes
  enum Mode {
    WIREFRAME,
    FACE,
    MULTICOLOURED
  };

  enum Transform {
    XAXIS,
    YAXIS,
    ZAXIS,
    SCALE
  };

  // A useful function that forces this widget to rerender. If you
  // want to render a new frame, do not call on_expose_event
  // directly. Instead call this, which will cause an on_expose_event
  // call when the time is right.
  void invalidate();

  void set_mode(Mode mode);
  void render_drawing_mode();

  // drawing mode methods
  void wireframe_mode();
  void face_mode();
  void multicoloured_mode();
  void new_game();

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
  // Takes params for x, y z coords and rgb values for cube colour
  void draw_cube(double x, double y, double z, double r, double g, double b, double a);
  void draw_piece_0(double x, double y, double z, double r, double g, double b, double a);
  void draw_piece_1(double x, double y, double z, double r, double g, double b, double a);
  void draw_piece_2(double x, double y, double z, double r, double g, double b, double a);
  void draw_piece_3(double x, double y, double z, double r, double g, double b, double a);
  void draw_piece_4(double x, double y, double z, double r, double g, double b, double a);
  void draw_piece_5(double x, double y, double z, double r, double g, double b, double a);
  void draw_piece_6(double x, double y, double z, double r, double g, double b, double a);
  void render_well(int width, int height);
  Mode m_mode; // drawing mode
  Transform m_transform;

  Game m_game(int width = 10, int height = 20);

  int x_origin;
  int scale_factor;
};

#endif
