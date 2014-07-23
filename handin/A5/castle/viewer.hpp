#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <GL/glew.h>
#include <gtkmm.h>
#include <gtkglmm.h>
#include <GL/glu.h>

#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "scene.hpp"
#include "primitive.hpp"
#include "algebra.hpp"
#include "trackball.hpp"
#include "events.hpp"
#include "material.hpp"
#include "a2.hpp"
#include "texture.hpp"
#include "celshading.hpp"
#include "bouncingball.hpp"
#include "glCamera.h"
#include "SoundManager.h"


using std::vector;

extern bool SHOW_BALLS, SHOW_LENSFLARE;

// The "main" OpenGL widget
class Viewer : public Gtk::GL::DrawingArea {


  public:
    Viewer();
    virtual ~Viewer();

    // camera view
    void keypress();
    Vector3D m_camera_scale;
    Vector3D m_camera_rotate;
    Vector3D m_camera_translate;

    enum Mode {
        ROTATE,
        TRANSLATE,
        SCALE
    };

    Mode m_mode;
    void set_mode(Mode mode) { m_mode = mode; }

    enum Keypress {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    Keypress m_keypress;
    void set_keypress(Keypress kp) { m_keypress = kp; }
    void fly_camera();

    // GLEW
    bool glew_init;
    void init_glew();

    // lighting
    void init_light();

    //trackball
    void init_trackball();

    // gl_settings
    void gl_settings();

    // lens flare
    void lens_flare();
    void init_lens_flare();
    glCamera *cam;
    void toggle_lensflare();

    // collisions
    void bouncing_balls();
    void toggle_balls();
    bool balls_inited;

    // sounds
    int ms_up, ms_down, ms_left, ms_right, ms_upz, ms_downz, ms_bg, ms_kp;
    bool m_sound, m_bg_playing, m_bg_resume;
    void play_bg();
    void toggle_sound();

    // timer
   void start_timer(int tick);
   bool tick_handler();
   int m_totaltime, m_tick;

    // A useful function that forces this widget to rerender. If you
    // want to render a new frame, do not call on_expose_event
    // directly. Instead call this, which will cause an on_expose_event
    // call when the time is right.
    void invalidate();

    // app menu
    void reset_position();
    void reset_orientation();
    void reset_joints();
    void reset_all();

    // options menu
    enum Option {
        CIRCLE,
        Z_BUFFER,
        BACKFACE_CULL,
        FRONTFACE_CULL,
        FRONTANDBACK_CULL
    };

    void set_options(Option option);

    // nodes

    SceneNode *m_scenenode;
    GeometryNode *m_geonode;

    // others
    bool m_circle;
    bool m_zbuffer;
    bool m_backface_cull;
    bool m_frontface_cull;


    int x_origin;
    int y_origin;
    int m_width;
    int m_height;

    int m_axis_dir;

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

    // Draw a circle for the trackball, with OpenGL commands.
    // Assumes the context for the viewer is active.
    void draw_trackball_circle();

private:
};

#endif
