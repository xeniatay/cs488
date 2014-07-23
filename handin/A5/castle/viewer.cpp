#include "viewer.hpp"

#include "collisions.hpp"

using namespace std;

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

  glew_init = false;
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

  // read shader file
  read_shader();

  // timer
  //start_timer(100);

  // bouncingball
  //reshape_bball(m_width, m_height);

  // collisions
  srand((unsigned int)time(0)); //Seed the random number generator
  _octree = new Octree(Vec3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2), Vec3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2), 1);
  createBalls();
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

  gl_settings();

  // Change to model view for drawing
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  init_glew();
  init_light();
  init_trackball();
  fly_camera();

  // Draw scene
  m_scenenode->walk_gl();


  // Lens Flare: translucent box overlay
  /*
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_SRC_ALPHA);
  glColor4f(0.1, 0.1, 0.1, 0.7);

  GLfloat center_x = m_width * 0.5f;
  GLfloat center_y = m_height * 0.5f;

  // screenPos is the onscreen position of the sun
  // mind that the pixel coordinates have been scaled
  // to fit our viewport
  GLfloat screenPos_x = 200;
  GLfloat screenPos_y = 100;
  cerr << center_x << " " << center_y << endl;
  GLfloat dx = center_x - screenPos_x;
  GLfloat dy = center_y - screenPos_y;
  GLfloat len = sqrt(dx * dx + dy * dy);

  // normalize the vector
  GLfloat vx = dx / len;
  GLfloat vy = dy / len;

  // choose a spacing between elements
  dx = vx * 0.4f;
  dy = vy * 0.4f;
  cerr << "vx, vy, len" << vx <<", " << vy << ", " << len << endl;

  // note that we already are in 2D (orthogonal)
  // mode here
  glPushMatrix();
  //glTranslatef(screenPos_x, screenPos_y, 0);

  int numElements = 3;
  // travel down the line and draw the elements
  for(int i = 0; i < numElements; i++) {
    cerr << "Drawing lens flare at " << dx <<", " << dy << ", " << " 0" << endl;
    glTranslatef(dx, dy, 0);
    glScaled(1.3, 1.3, 1);
    // Drawing a polygonnn
     glBegin(GL_POLYGON);            // These vertices form a closed polygon
        glColor4f(1.0f, 1.0f, 0.0f, 0.7f); // Yellow
        glVertex2f(0.4f, 0.2f);
        glVertex2f(0.6f, 0.2f);
        glVertex2f(0.7f, 0.4f);
        glVertex2f(0.6f, 0.6f);
        glVertex2f(0.4f, 0.6f);
        glVertex2f(0.3f, 0.4f);
     glEnd();
    //draw_flare_element();
  }
  glPopMatrix();
  //glRecti(0, 0, m_width, m_height);
  glDisable(GL_BLEND);
  */

  // celshading monster
  //DrawGLScene();

  //display_bball();

  // collisions
  /*
  initRendering();
  update(0);
  drawScene();
  */

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

  //glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

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
  } else if (option == FRONTANDBACK_CULL) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT_AND_BACK);
  } else if (option == CIRCLE) {
    m_circle = true;
  }

  invalidate();

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

/// https://www.cse.msu.edu/~cse872/tutorial3.html
void Viewer::init_light() {
  GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
  // trying to make it like a sun
  GLfloat position[] = {-m_width / 2, m_height, 1000, 1.0};
  GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {50.0};

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_POSITION, position);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  /*
GLfloat white[] = {0.8f, 0.8f, 0.8f, 1.0f};
GLfloat cyan[] = {0.f, .8f, .8f, 1.f};
glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
glMaterialfv(GL_FRONT, GL_SPECULAR, white);
GLfloat shininess[] = {50};
glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
*/

}

void Viewer::fly_camera() {
  glScaled(m_camera_scale[0], m_camera_scale[1], m_camera_scale[2]);
  glRotated(m_camera_rotate[0], 1, 0, 0);
  glRotated(m_camera_rotate[1], 0, 1, 0);
  glRotated(m_camera_rotate[2], 0, 0, 1);
  glTranslated(m_camera_translate[0], m_camera_translate[1], m_camera_translate[2]);
}

void Viewer::init_glew() {
  // init GLEW once
  if (!glew_init) {
    glewInit();
    glew_init = true;

    if (glewIsSupported("GL_VERSION_2_0"))
      printf("Ready for OpenGL 2.0\n");
    else {
      printf("OpenGL 2.0 not supported\n");
    }
  }
}

void Viewer::init_trackball() {
  Matrix *mRot = getMRot();
  Matrix *mTrans = getMTrans();
  glLoadMatrixd((GLdouble *) mTrans);
  glMultMatrixd((GLdouble *) mRot);
  if (m_circle) { draw_trackball_circle(); }
}

void Viewer::gl_settings() {
  gluPerspective(40.0, (GLfloat)get_width()/(GLfloat)get_height(), 0.01, 1000.0);

  // Clear framebuffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);     // Realy Nice perspective calculations

  // Enable depth test done in on_realize
  // glEnable(GL_DEPTH_TEST);

}

void Viewer::start_timer(int tick) {
  // Start ticker
  m_totaltime = 0;
  m_tick = tick;
  sigc::slot0<bool> tick_slot = sigc::mem_fun(this, &Viewer::tick_handler);
  Glib::signal_timeout().connect(tick_slot, m_tick);
}

bool Viewer::tick_handler() {
  m_totaltime += m_tick;
  // animate here
  invalidate();

  return true;
}