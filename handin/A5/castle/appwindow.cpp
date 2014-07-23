#include "appwindow.hpp"

AppWindow::AppWindow()
{
  set_title("Advanced Ergonomics Laboratory");

  // A utility class for constructing things that go into menus, which
  // we'll set up next.
  using Gtk::Menu_Helpers::MenuElem;
  using Gtk::Menu_Helpers::RadioMenuElem;
  using Gtk::Menu_Helpers::CheckMenuElem;

  // Set up the application menu
  // The slot we use here just causes AppWindow::hide() on this,
  // which shuts down the application.
  //m_menu_app.items().push_back(MenuElem("_Reset Position (I)", sigc::mem_fun(m_viewer, &Viewer::reset_position)));
  //m_menu_app.items().push_back(MenuElem("_Reset Orientation (O)", sigc::mem_fun(m_viewer, &Viewer::reset_orientation)));
  //m_menu_app.items().push_back(MenuElem("_Reset Joints (N)", sigc::mem_fun(m_viewer, &Viewer::reset_joints)));
  m_menu_app.items().push_back(MenuElem("_Reset All (A)", sigc::mem_fun(m_viewer, &Viewer::reset_all)));
  m_menu_app.items().push_back(MenuElem("_Quit (Q)", sigc::mem_fun(*this, &AppWindow::hide)));

  // Set up the mode radio menu group
  sigc::slot1<void, Viewer::Mode> mode_slot = sigc::mem_fun(m_viewer, &Viewer::set_mode);
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_SCALE (S)", sigc::bind(mode_slot, Viewer::SCALE)));
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_TRANSLATE (T)", sigc::bind(mode_slot, Viewer::TRANSLATE)));
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_ROTATE (R)", sigc::bind(mode_slot, Viewer::ROTATE)));

  // Set up the options checklist menu group
  sigc::slot1<void, Viewer::Option> option_slot = sigc::mem_fun(m_viewer, &Viewer::set_options);
  m_menu_options.items().push_back(CheckMenuElem("_Circle (C)", sigc::bind(option_slot, Viewer::CIRCLE)));
  m_menu_options.items().push_back(CheckMenuElem("_Z-buffer (Z)", sigc::bind(option_slot, Viewer::Z_BUFFER)));
  m_menu_options.items().push_back(CheckMenuElem("_Backface Cull (B)", sigc::bind(option_slot, Viewer::BACKFACE_CULL)));
  m_menu_options.items().push_back(CheckMenuElem("_Frontface Cull (F)", sigc::bind(option_slot, Viewer::FRONTFACE_CULL)));
  m_menu_options.items().push_back(CheckMenuElem("_Front and Backface Cull ()", sigc::bind(option_slot, Viewer::FRONTANDBACK_CULL)));

  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Mode", m_menu_mode));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Options", m_menu_options));

  // Pack in our widgets

  // First add the vertical box as our single "top" widget
  add(m_vbox);

  // Put the menubar on the top, and make it as small as possible
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

  // Put the viewer below the menubar. pack_start "grows" the widget
  // by default, so it'll take up the rest of the window.
  m_viewer.set_size_request(1000, 700);
  m_vbox.pack_start(m_viewer);

  show_all();
}

bool AppWindow::on_key_press_event( GdkEventKey *ev ) {
  // This is a convenient place to handle non-shortcut
  // keys.  You'll want to look at ev->keyval.

  // An example key; delete and replace with the
  // keys you want to process

  std::cerr << "Stub: Key pressed! " << ev->keyval << std::endl;

  if (ev->keyval == 'q' || ev->keyval == 'Q') {
    hide();
  } else if (ev->keyval == 'a' || ev->keyval == 'A') {
    m_viewer.reset_all();
  } else if (ev->keyval == 'z' || ev->keyval == 'Z') {
    //m_viewer.set_options(m_viewer.Z_BUFFER);
  } else if (ev->keyval == 'b' || ev->keyval == 'B') {
    m_viewer.set_options(m_viewer.BACKFACE_CULL);
  } else if (ev->keyval == 'f' || ev->keyval == 'F') {
    m_viewer.set_options(m_viewer.FRONTFACE_CULL);
  } else if (ev->keyval == 'r' || ev->keyval == 'R') {
    m_viewer.set_mode(m_viewer.ROTATE);
  } else if (ev->keyval == 't' || ev->keyval == 'T') {
    m_viewer.set_mode(m_viewer.TRANSLATE);
  } else if (ev->keyval == 's' || ev->keyval == 'S') {
    m_viewer.set_mode(m_viewer.SCALE);
  } else if (ev->keyval == 'p' || ev->keyval == 'P') {
    m_viewer.toggle_sound();
  } else if (ev->keyval == 'o' || ev->keyval == 'O') {
    m_viewer.play_bg();
  } else if (ev->keyval == 65362) {
    m_viewer.set_keypress(m_viewer.UP);
    m_viewer.keypress();
  } else if (ev->keyval == 65364) {
    m_viewer.set_keypress(m_viewer.DOWN);
    m_viewer.keypress();
  } else if (ev->keyval == 65361) {
    m_viewer.set_keypress(m_viewer.LEFT);
    m_viewer.keypress();
  } else if (ev->keyval == 65363) {
    m_viewer.set_keypress(m_viewer.RIGHT);
    m_viewer.keypress();
  }

  //return true;
  return Gtk::Window::on_key_press_event( ev );
}
