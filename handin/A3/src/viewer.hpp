#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include "scene.hpp"
#include "primitive.hpp"
#include <vector>

using std::vector;

// The "main" OpenGL widget
class Viewer : public Gtk::GL::DrawingArea {


  public:
    Viewer();
    virtual ~Viewer();

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

    // mode menu
    enum Mode {
        POSITION_OR_ORIENTATION,
        JOINTS
    };

    void set_mode(Mode mode) { m_mode = mode; }

    // edit menu
    void undo();
    void redo();

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
    Mode m_mode;
    Option m_circle;
    Option m_zbuffer;
    Option m_backface_cull;
    Option m_frontface_cull;


    int x_origin;
    int y_origin;
    int m_width;
    int m_height;

    // picking
    enum Picking {
        ROOT,
        TORSO,
        SHOULDER,
        LEFT_UPPER_ARM,
        LEFT_FOREARM,
        LEFT_HAND,
        RIGHT_UPPER_ARM,
        RIGHT_FOREARM,
        RIGHT_HAND,
        NECK,
        HEAD,
        HIPS,
        LEFT_THIGH,
        LEFT_CALF,
        LEFT_FOOT,
        RIGHT_THIGH,
        RIGHT_CALF,
        RIGHT_FOOT
    };

    int numLimbs;

    void picking_in_select_mode();
    void set_pickings(Picking picked);

    vector <bool> picked_list;

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
