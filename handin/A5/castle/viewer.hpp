#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <vector>

#include "scene.hpp"
#include "primitive.hpp"
#include "algebra.hpp"
#include "trackball.hpp"
#include "events.hpp"
#include "material.hpp"
#include "image.hpp"
#include "a2.hpp"
#include "perlinnoise.hpp"

using std::vector;

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

    // lighting
    void init_light();

    // texture mapping
    void map_texture(GLuint texture);
    GLuint t_castle_wall;
    GLuint texture_count;

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
        FRONTFACE_CULL
    };

    void set_options(Option option);

    // nodes

    SceneNode *m_scenenode;
    GeometryNode *m_geonode;

    // others
    bool m_circle;
    Option m_zbuffer;
    Option m_backface_cull;
    Option m_frontface_cull;


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
