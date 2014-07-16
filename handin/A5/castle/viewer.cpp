#include "viewer.hpp"
#include "castle_terrain.hpp"

using std::cerr;
using std::endl;
using std::string;

typedef std::list<SceneNode*> SN;
extern SN all_scenenodes;
extern vector <SceneNode*> scenenodes_v;

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
  glClearColor( 1.0, 1.0, 1.0, 0.0 );
  glEnable(GL_DEPTH_TEST);

  gldrawable->gl_end();

  //cerr << "On realize!" << endl;
  reset_all();

  // default mode
  m_mode = SCALE;
  texture_count = 0;
  map_texture(t_castle_wall);
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  //cerr << "On expose event!" << endl;
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();
  if (!gldrawable) return false;
  if (!gldrawable->gl_begin(get_gl_context())) return false;

  // Set up for perspective drawing
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, get_width(), get_height());
  gluPerspective(40.0, (GLfloat)get_width()/(GLfloat)get_height(), 0.01, 1000.0);

  // Change to model view for drawing
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Clear framebuffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Enable texture
  glEnable( GL_TEXTURE_2D );

  // Set up lighting
  //init_light();

  // Trackball
  Matrix *mRot = getMRot();
  Matrix *mTrans = getMTrans();
  glLoadMatrixd((GLdouble *) mTrans);
  glMultMatrixd((GLdouble *) mRot);
  if (m_circle) { draw_trackball_circle(); }

  // Camera view
  fly_camera();

  // Draw scene
  m_scenenode->walk_gl();
  //texture_test();

  ct_init();
  ct_display();
  ct_reshape();

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
  //std::cerr << "Stub: Button " << event->button << " pressed" << std::endl;

  x_origin = event->x;
  y_origin = event->y;

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

  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  //std::cerr << "Stub: Button " << event->button << " released" << std::endl;

  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  //std::cerr << "Stub: Motion at " << event->x << ", " << event->y << std::endl;

  m_axis_dir = ( (event->x - x_origin) < 0) ? -1 : 1;

  float x_current = event->x,
        y_current = event->y;

  // trackball stuff
  vPerformTransfo((float)x_origin, x_current, (float)y_origin, y_current);

  x_origin = x_current;
  y_origin = y_current;

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

void Viewer::reset_all() {
  m_camera_scale = Vector3D(1, 1, 1);
  m_camera_translate = Vector3D();
  m_camera_rotate = Vector3D();

  m_circle = false;
  m_axis_dir = 1;
  x_origin = 0;
  y_origin = 0;
  m_width = get_width();
  m_height = get_height();

  m_scenenode = all_scenenodes.front();
  m_geonode = all_geonodes.front();
  //cerr << "M_SCENENODE: " << m_scenenode->m_name << endl;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float curMatrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, curMatrix);

  resetMRot();
  resetMTrans();

  vToggleDir(DIR_NONE);

/*
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
*/

  invalidate();
}

void Viewer::set_options(Option option) {

  if (option == Z_BUFFER) {
    glEnable(GL_DEPTH_TEST);
  } else if (option == FRONTFACE_CULL) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
  } else if (option == BACKFACE_CULL) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  } else if (option == CIRCLE) {
    m_circle = true;
  }

  invalidate();

}

/*
void Viewer::load_image(Image img, string filename, int width, int height, int depth) {
  img.loadPng(filename);
}
*/

// from: http://www.nullterminator.net/gltexture.html
void Viewer::map_texture(GLuint texture) {
  texture_count++;

  // allocate a texture name
  glGenTextures( texture_count, &texture );

  // select our current texture
  glBindTexture( GL_TEXTURE_2D, texture );

  // select modulate to mix texture with color for shading
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

  // when texture area is small, bilinear filter the closest mipmap
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                 GL_LINEAR_MIPMAP_NEAREST );
  // when texture area is large, bilinear filter the original
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // the texture wraps over at the edges (repeat)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  int width = 300, height = 300;
  Image img(width, height, 3);
  string filename = "assets/castle_wall_texture_1.png";
  img.loadPng(filename);

  // build our texture mipmaps
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, img.data() );

}

void Viewer::keypress() {

  // determine direction of transformation
  int transform_dir = (m_keypress == DOWN || m_keypress == LEFT) ? -1 : 1;

  if (m_mode == SCALE) {
    double s = 0.15 * transform_dir;
    Vector3D m_s = m_camera_scale;
    m_camera_scale = Vector3D(m_s[0] + s, m_s[1] + s, m_s[2] + s);

  } else if (m_mode == TRANSLATE) {
    double t = 0.5 * transform_dir;
    Vector3D m_t = m_camera_translate;

    if (m_keypress == UP || m_keypress == DOWN) {
      m_camera_translate = Vector3D(m_t[0], m_t[1], m_t[2] + t);
    } else {
      m_camera_translate = Vector3D(m_t[0] + t, m_t[1], m_t[2]);
    }

  } else if (m_mode == ROTATE) {
    double r = 2.0 * transform_dir;
    Vector3D m_r = m_camera_rotate;

    if (m_keypress == UP || m_keypress == DOWN) {
      m_camera_rotate = Vector3D(m_r[0] + r, m_r[1], m_r[2]);
    } else {
      m_camera_rotate = Vector3D(m_r[0], m_r[1] + r, m_r[2]);
    }
  }

  invalidate();
}

void Viewer::init_light() {
  GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat position[] = {0.0, 0.0, 2.0, 1.0};
  GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {50.0};

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_POSITION, position);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void Viewer::fly_camera() {
  glScaled(m_camera_scale[0], m_camera_scale[1], m_camera_scale[2]);
  glRotated(m_camera_rotate[0], 1, 0, 0);
  glRotated(m_camera_rotate[1], 0, 1, 0);
  glRotated(m_camera_rotate[2], 0, 0, 1);
  glTranslated(m_camera_translate[0], m_camera_translate[1], m_camera_translate[2]);
}

void texture_test() {
  // map texture to shape
  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, 1);
  glBegin( GL_QUADS );
  glTexCoord2d(0.0,0.0); glVertex2d(0.0,0.0);
  glTexCoord2d(10.0,0.0); glVertex2d(10.0,0.0);
  glTexCoord2d(10.0,10.0); glVertex2d(10.0,10.0);
  glTexCoord2d(0.0,10.0); glVertex2d(0.0,10.0);
  glEnd();
}