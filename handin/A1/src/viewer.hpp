#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <list>
#include <gtkmm.h>
#include <gtkglmm.h>
#include "game.hpp"

struct TetrisPiece {
  int id, index;
  double x, y, z, rotation;
};

// The "main" OpenGL widget
class Viewer : public Gtk::GL::DrawingArea {
Game m_game;

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

  enum Speed {
    SLOW,
    MEDIUM,
    FAST
  };

  enum KeyPress {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    SPACE
  };

  // A useful function that forces this widget to rerender. If you
  // want to render a new frame, do not call on_expose_event
  // directly. Instead call this, which will cause an on_expose_event
  // call when the time is right.
  void invalidate();

  void set_mode(Mode mode);
  void set_speed(Speed speed);
  void start_timer();
  void render_drawing_mode();

  // drawing mode methods
  void wireframe_mode();
  void face_mode();
  void multicoloured_mode();
  void new_game();
  void reset();

  // Takes params for x, y z coords and rgb values for cube colour
  void draw_cube(double x, double y, double z, double r, double g, double b, double a);
  void draw_piece_0(double x, double y, double z, double r = 0.2, double g = 0.8, double b = 0, double a = 0.8);
  void draw_piece_1(double x, double y, double z, double r = 0, double g = 0.2, double b = 0.8, double a = 0.8);
  void draw_piece_2(double x, double y, double z, double r = 0.2, double g = 0, double b = 0.8, double a = 0.8);
  void draw_piece_3(double x, double y, double z, double r = 0.5, double g = 0.9, double b = 0, double a = 0.8);
  void draw_piece_4(double x, double y, double z, double r = 0, double g = 0.5, double b = 0.9, double a = 0.8);
  void draw_piece_5(double x, double y, double z, double r = 0.5, double g = 0, double b = 0.9, double a = 0.8);
  void draw_piece_6(double x, double y, double z, double r = 0.5, double g = 0, double b = 0.9, double a = 0.8);
  void draw_piece_7(double x, double y, double z, double r = .7, double g = .3, double b = .8, double a = 0.8);
  void draw_pieces();
  void add_new_piece();
  void render_well(int width, int height);
  void press_left();
  void press_right();
  void press_up();
  void press_down();
  void press_space();
  void move_piece();

  bool tick_handler();
  void tick();

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
  Mode m_mode; // drawing mode
  int m_speed;
  int total_time;
  Transform m_transform;

  int x_origin;
  int scale_factor;

  int m_height;
  int m_width;
  int m_xorigin;
  int collapsed;

  std::list<TetrisPiece*> tetris_pieces;
};

#endif
