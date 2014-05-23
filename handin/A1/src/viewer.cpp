// Xenia Tay
// 20396769
// xzytay

#include "viewer.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>

Viewer::Viewer() : m_game(10, 20)
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

  start_timer();
  new_game();
  matrix_layers = 0;
  scale_factor = 0;
  m_speed = 1000; // default
  persist = false;
  mouse_motion = false;
  mouse_release = false;
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  render_drawing_mode();

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

  render_well(m_width, m_height);

  draw_pieces();

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

  return true;
}

bool Viewer::on_button_press_event(GdkEventButton* event)
{
  //std::cerr << "Stub: Button " << event->button << " pressed" << std::endl;
  //std::cerr << "Button: " << event->button << std::endl;

  if (persist = true) {
    persist = false;
    on_expose_event(NULL);
  }

  glPushMatrix();
  matrix_layers++;

  switch (event->button) {
    case 1:
      m_transform = Viewer::XAXIS;
      break;
    case 2:
      m_transform = Viewer::YAXIS;
      break;
    case 3:
      m_transform = Viewer::ZAXIS;
      break;
    default:
      break;
  }

  if (event->state & GDK_SHIFT_MASK) {
    m_transform = Viewer::SCALE;
  }

  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  //std::cerr << "Stub: Button " << event->button << " released" << std::endl;
  mouse_release = true;
  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  //std::cerr << "Stub: Motion at " << event->x << ", " << event->y << std::endl;
  mouse_motion = true;

  m_transform_dir = ( (event->x - x_origin) < 0) ? -1 : 1;

  if (m_transform != Viewer::SCALE) {
    glTranslated(5.0, 12.0, 0.0);
    glTranslated(-1 * m_width, -1 * m_height, 0);
  }

  if (m_transform == Viewer::XAXIS) {
    glRotated(m_transform_dir * 0.8, 1, 0, 0);
  } else if (m_transform == Viewer::YAXIS) {
    glRotated(m_transform_dir * 0.8, 0, 1, 0);
  } else if (m_transform == Viewer::ZAXIS) {
    glRotated(m_transform_dir * 0.8, 0, 0, 1);
  } else if (m_transform == Viewer::SCALE) {
    if (m_transform_dir == 1 && scale_factor < 50) {
      glScaled(1.02, 1.02, 1);
      scale_factor++;
    } else if (scale_factor > -50) {
      glScaled(0.98, 0.98, 1);
      scale_factor--;
    }
    //std::cerr << "scale factor: " << scale_factor << std::endl;
  }

  if (m_transform != Viewer::SCALE) {
    glTranslated(m_width, m_height, 0);
    glTranslated(-5.0, -12.0, 0.0);
  }

  x_origin = event->x;
  on_expose_event(NULL);
  return true;
}

// Params: x, y z coords and r, g, b colour values
// Draws a unit cube (width = 1, height = 1, breadth = 1)
void Viewer::draw_cube(double x, double y, double z, double r = 0, double g = 0, double b = 0, double a = 0.8) {
  bool multicolour = (m_mode == MULTICOLOURED);
  double r_val = 0, g_val = 0, b_val = 0;

  glBegin(GL_QUADS);

    r_val = fmod((r + 0.4), 1);
    multicolour ? glColor4d( r_val, g, b, a) : glColor4d(r, g, b, a);
    glVertex3d (x, y, z); // bottom left (front)
    glVertex3d (x, y + 1, z); // top left (front)
    glVertex3d (x + 1, y + 1, z); // top right (front)
    glVertex3d (x + 1, y, z); // bottom right (front)

    r_val = fmod((r + 0.8), 1);
    multicolour ? glColor4d( r_val, g, b, a) : glColor4d(r, g, b, a);
    glVertex3d (x, y, z + 1); // bottom left (back)
    glVertex3d (x, y + 1, z + 1); // top left (back)
    glVertex3d (x + 1, y + 1, z + 1); // top right (back)
    glVertex3d (x + 1, y, z + 1); // bottom right (back)

    g_val = fmod((g + 0.4), 1);
    multicolour ? glColor4d( r, g_val, b, a) : glColor4d(r, g, b, a);
    glVertex3d (x, y, z); // bottom left (left)
    glVertex3d (x, y + 1, z); // top left (left)
    glVertex3d (x, y + 1, z + 1); // top right (left)
    glVertex3d (x, y, z + 1); // bottom right (left)

    g_val = fmod((r + 0.8), 1);
    multicolour ? glColor4d( r, g_val, b, a) : glColor4d(r, g, b, a);
    glVertex3d (x + 1, y, z); // bottom left (right)
    glVertex3d (x + 1, y + 1, z); // top left (right)
    glVertex3d (x + 1, y + 1, z + 1); // top right (right)
    glVertex3d (x + 1, y, z + 1); // bottom right (right)

    b_val = fmod((b + 0.4), 1);
    multicolour ? glColor4d( r, g, b_val, a) : glColor4d(r, g, b, a);
    glVertex3d (x, y + 1, z); // bottom left (top)
    glVertex3d (x, y + 1, z + 1); // top left (top)
    glVertex3d (x + 1, y + 1, z + 1); // top right (top)
    glVertex3d (x + 1, y + 1, z); // bottom right (top)

    b_val = fmod((b + 0.8), 1);
    multicolour ? glColor4d( r, g, b_val, a) : glColor4d(r, g, b, a);
    glVertex3d (x, y, z); // bottom left (bottom)
    glVertex3d (x, y, z + 1); // top left (bottom)
    glVertex3d (x + 1, y, z + 1); // top right (bottom)
    glVertex3d (x + 1, y, z); // bottom right (bottom)
  glEnd();
}

/*
  Piece 0:
  ".x.."
  ".x.."
  ".x.."
  ".x.."
*/
void Viewer::draw_piece_0(double x, double y, double z, double r, double g, double b, double a) {
  draw_cube(x + 1, y, z, r, g, b, a);
  draw_cube(x + 1, y - 1, z, r, g, b, a);
  draw_cube(x + 1, y - 2, z, r, g, b, a);
  draw_cube(x + 1, y - 3, z, r, g, b, a);
}

/* Piece 1:
  "...."
  ".xx."
  ".x.."
  ".x.."
*/
void Viewer::draw_piece_1(double x, double y, double z, double r, double g, double b, double a) {
  draw_cube(x + 1, y - 1, z, r, g, b, a);
  draw_cube(x + 2, y - 1, z, r, g, b, a);
  draw_cube(x + 1, y - 2, z, r, g, b, a);
  draw_cube(x + 1, y - 3, z, r, g, b, a);
}

/* Piece 2:
  "...."
  ".xx."
  "..x."
  "..x."
*/
void Viewer::draw_piece_2(double x, double y, double z, double r, double g, double b, double a) {
  draw_cube(x + 1, y - 1, z, r, g, b, a);
  draw_cube(x + 2, y - 1, z, r, g, b, a);
  draw_cube(x + 2, y - 2, z, r, g, b, a);
  draw_cube(x + 2, y - 3, z, r, g, b, a);
}

/* Piece 3:
  "...."
  ".x.."
  ".xx."
  "..x."
*/
void Viewer::draw_piece_3(double x, double y, double z, double r, double g, double b, double a) {
  // rotate piece
  draw_cube(x + 1, y - 1, z, r, g, b, a);
  draw_cube(x + 1, y - 2, z, r, g, b, a);
  draw_cube(x + 2, y - 2, z, r, g, b, a);
  draw_cube(x + 2, y - 3, z, r, g, b, a);
}

/* Piece 4:
  "...."
  "..x."
  ".xx."
  ".x.."
*/
void Viewer::draw_piece_4(double x, double y, double z, double r, double g, double b, double a) {
  draw_cube(x + 2, y - 1, z, r, g, b, a);
  draw_cube(x + 1, y - 2, z, r, g, b, a);
  draw_cube(x + 2, y - 2, z, r, g, b, a);
  draw_cube(x + 1, y - 3, z, r, g, b, a);
}

/* Piece 5:
  "...."
  "xxx."
  ".x.."
  "...."
*/
void Viewer::draw_piece_5(double x, double y, double z, double r, double g, double b, double a) {
  draw_cube(x, y - 1, z, r, g, b, a);
  draw_cube(x + 1, y - 1, z, r, g, b, a);
  draw_cube(x + 2, y - 1, z, r, g, b, a);
  draw_cube(x + 1, y - 2, z, r, g, b, a);
}

/* Piece 6:
  "...."
  ".xx."
  ".xx."
  "...."
*/
void Viewer::draw_piece_6(double x, double y, double z, double r, double g, double b, double a) {
  draw_cube(x + 1, y - 1, z, r, g, b, a);
  draw_cube(x + 2, y - 1, z, r, g, b, a);
  draw_cube(x + 1, y - 2, z, r, g, b, a);
  draw_cube(x + 2, y - 2, z, r, g, b, a);
}

/* Piece 7:
  "...."
  ".xx."
  ".x.."
  "...."
*/
void Viewer::draw_piece_7(double x, double y, double z, double r, double g, double b, double a) {
  draw_cube(x + 1, y - 1, z, r, g, b, a);
  draw_cube(x + 2, y - 1, z, r, g, b, a);
  draw_cube(x + 1, y - 2, z, r, g, b, a);
}

// Params: width, height
// Draws a U-shaped well of unit cubes
void Viewer::render_well(int width, int height) {
  int i = 0;

  for (i = m_xorigin; i < height; i++) {
    draw_cube(-1, i, 0, .1, .1, .1);
    draw_cube(width, i, 0, .1, .1, .1);
  }

  for (i = 0; i < width; i++) {
    draw_cube(i, m_xorigin, 0, .1, .1, .1);
  }
}

// set game drawing mode
void Viewer::set_mode(Mode mode) {
  m_mode = mode;
  on_expose_event(NULL);
}

void Viewer::render_drawing_mode() {
  switch (m_mode) {
    case WIREFRAME:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
    case FACE:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    case MULTICOLOURED:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    default:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
  }
}

void Viewer::new_game() {
  tetris_pieces.clear();
  m_game.reset();

  x_origin = 0;
  total_time = 0;
  add_new_piece();
  m_height = 20;
  m_width = 10;
  m_xorigin = -1;
  collapsed = 0;
  collapsed_pieces = 0;

  if (game_over == true) {
    for (int i = 0; i < gameover_layers; i++) {
      glPopMatrix();
    }
  }

  gameover_layers = 0;
  game_over = false;

  on_expose_event(NULL);
}

void Viewer::reset() {
  for (int i = 0; i < matrix_layers; i++) {
    glPopMatrix();
  }
  matrix_layers = 0;
  scale_factor = 0;
  on_expose_event(NULL);
}

void Viewer::add_new_piece() {
  TetrisPiece *new_piece = new TetrisPiece;
  new_piece->id = tetris_pieces.size();
  new_piece->index = m_game.current_piece();
  //new_piece->x = m_game.px();
  //new_piece->y = m_game.py();
  new_piece->x = 3;
  new_piece->y = 23;
  new_piece->z = 0;
  new_piece->rotation = 0;
  new_piece->ytransform = 0;

  tetris_pieces.push_back(new_piece);
  //std::cerr << "Total number of pieces: " << tetris_pieces.size() << std::endl;
}

void Viewer::draw_pieces() {
  // Iterate over the stored pieces and ask them to draw themselves.
  //

  for( std::list<TetrisPiece*>::iterator i = tetris_pieces.begin(); i != tetris_pieces.end(); ++i ) {
    TetrisPiece *piece = (*i);

    if (piece->id < collapsed_pieces) {
      // hide rows that are won
      glTranslated(0, -1 * collapsed, 0);
    }

    double tx = 0, ty = 0, tz = 0;

    // calculate transformation
    if ( (piece->rotation == 270) || (piece->rotation == -90) ) {
      tx = ((piece->y - piece->x) * -1) + 3;
      ty = piece->x + piece->y + 1;
    } else if ( (piece->rotation == 180) || (piece->rotation == -180) ) {
      tx = 2 * piece->x + 4;
      ty = 2 * piece->y - 2;
    } else if ( (piece->rotation == 90) || (piece->rotation == -270) ) {
      tx = piece->y + piece->x + 1;
      ty = piece->y - piece->x - 3;
    }

    glTranslated(tx, ty, tz);
    glRotated(piece->rotation, 0, 0, 1.0);

    switch(piece->index) {
      case 0: draw_piece_0(piece->x, piece->y, piece->z); break;
      case 1: draw_piece_1(piece->x, piece->y, piece->z); break;
      case 2: draw_piece_2(piece->x, piece->y, piece->z); break;
      case 3: draw_piece_3(piece->x, piece->y, piece->z); break;
      case 4: draw_piece_4(piece->x, piece->y, piece->z); break;
      case 5: draw_piece_5(piece->x, piece->y, piece->z); break;
      case 6: draw_piece_6(piece->x, piece->y, piece->z); break;
      case 7: draw_piece_7(piece->x, piece->y, piece->z); break;
      default: draw_piece_0(piece->x, piece->y, piece->z); break;
    }

    glRotated(-1 * piece->rotation, 0, 0, 1.0);
    glTranslated(-1 * tx, -1 * ty, tz);

    if (piece->id < collapsed_pieces) {
      // hide rows that are won
      glTranslated(0, collapsed, 0);
    }

  }

}

void Viewer::set_speed(Speed speed) {
  switch(speed) {
    case SLOW:
      m_speed = 1000;
      break;
    case MEDIUM:
      m_speed = 500;
      break;
    case FAST:
      m_speed = 100;
      break;
    default:
      m_speed = 1000;
      break;
  }
}

void Viewer::start_timer() {
  // Start ticker
  sigc::slot0<bool> tick_slot = sigc::mem_fun(this, &Viewer::tick_handler);
  Glib::signal_timeout().connect(tick_slot, 10);
}

bool Viewer::tick_handler() {
  total_time += 10;

  if (game_over) {
    glRotated(1, 0, 0, 1);
    on_expose_event(NULL);
  }

  if (total_time % m_speed == 0) {
    tick();
  }

  if (mouse_motion && mouse_release) {
    persist = true;
    std::cerr << "Persist is true" << std::endl;
  }

  mouse_motion = false;
  mouse_release = false;

  if (persist) {
    if (m_transform == Viewer::XAXIS) {
      glRotated(m_transform_dir * 1, 1, 0, 0);
    } else if (m_transform == Viewer::YAXIS) {
      glRotated(m_transform_dir * 1, 0, 1, 0);
    } else if (m_transform == Viewer::ZAXIS) {
      glRotated(m_transform_dir * 1, 0, 0, 1);
    }
    on_expose_event(NULL);
  }

  return true;
}

void Viewer::tick() {
  int tick_val = m_game.tick();


  switch(tick_val) {
    case 0:
      if (m_game.is_new_piece) {
        add_new_piece();
      } else {
        move_piece();
      }
      break;
    case 1: case 2: case 3: case 4:
      std::cerr << "TICK: " << tick_val << std::endl;
      move_piece(tick_val);
      collapse_rows(tick_val);
      add_new_piece();
      break;
    default:
      glPushMatrix();
      gameover_layers++;
      game_over = true;
      // game over;
      break;
  }

  on_expose_event(NULL);
}

void Viewer::collapse_rows(int rows) {
  collapsed += rows;
  collapsed_pieces = tetris_pieces.size();
}

void Viewer::press_left() {
  m_game.moveLeft();
  move_piece();
  //std::cerr << "hi left " << std::endl;
}

void Viewer::press_right() {
  m_game.moveRight();
  move_piece();
  //std::cerr << "hi right " << std::endl;
}

void Viewer::press_up() {
  bool can_rotate = m_game.rotateCW();
  if (can_rotate) {
    TetrisPiece *cur_piece = tetris_pieces.back();
    cur_piece->rotation = fmod((cur_piece->rotation - 90), 360);
    on_expose_event(NULL);
    //std::cerr << "Rotate CW: " << cur_piece->rotation << std::endl;
  }
}

void Viewer::press_down() {
  bool can_rotate = m_game.rotateCCW();
  if (can_rotate) {
    TetrisPiece *cur_piece = tetris_pieces.back();
    cur_piece->rotation = fmod((cur_piece->rotation + 90), 360);
    on_expose_event(NULL);
    //std::cerr << "Rotate CCW: " << cur_piece->rotation << std::endl;
  }
}

void Viewer::press_space() {
  m_game.drop();
  move_piece();
  //std::cerr << "Drop!" << std::endl;
}

void Viewer::move_piece() {
  TetrisPiece *cur_piece = tetris_pieces.back();
  cur_piece->x = m_game.px();
  cur_piece->y = m_game.py();
  std::cerr << "Move ============== x: " << cur_piece->x << " y: " << cur_piece->y << std::endl;
  on_expose_event(NULL);
}

void Viewer::move_piece(double collapse) {
  TetrisPiece *cur_piece = tetris_pieces.back();
  cur_piece->y = cur_piece->y - collapse + 1;
  std::cerr << "Move ============== x: " << cur_piece->x << " y: " << cur_piece->y << std::endl;
  on_expose_event(NULL);
}
