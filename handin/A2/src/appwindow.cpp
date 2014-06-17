// Xenia Tay
// 20396769
// xzytay

#include "appwindow.hpp"

AppWindow::AppWindow()
{
  set_title("CS488 Assignment Two");

  // A utility class for constructing things that go into menus, which
  // we'll set up next.
  using Gtk::Menu_Helpers::MenuElem;
  using Gtk::Menu_Helpers::RadioMenuElem;

  // Set up the application menu
  // The slot we use here just causes AppWindow::hide() on this,
  // which shuts down the application.
  m_menu_app.items().push_back(MenuElem("_Quit (Q)", sigc::mem_fun(*this, &AppWindow::hide)));
  m_menu_app.items().push_back(MenuElem("_Reset (A)", sigc::mem_fun(m_viewer, &Viewer::reset)));

  // Set up the draw mode radio menu group
  sigc::slot1<void, Viewer::Mode> mode_slot = sigc::mem_fun(m_viewer, &Viewer::set_mode);
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_Model Rotate (R)", sigc::bind(mode_slot, Viewer::MODEL_ROTATE)));
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_Model Translate (T)", sigc::bind(mode_slot, Viewer::MODEL_TRANSLATE)));
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_Model Scale (S)", sigc::bind(mode_slot, Viewer::MODEL_SCALE)));
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_View Rotate (O)", sigc::bind(mode_slot, Viewer::VIEW_ROTATE)));
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_View Translate (N)", sigc::bind(mode_slot, Viewer::VIEW_TRANSLATE)));
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_View Perspective (P)", sigc::bind(mode_slot, Viewer::VIEW_PERSPECTIVE)));
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_Viewport (V)", sigc::bind(mode_slot, Viewer::VIEWPORT)));

  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Mode", m_menu_mode));

  // Pack in our widgets

  // First add the vertical box as our single "top" widget
  add(m_vbox);

  // Put the menubar on the top, and make it as small as possible
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

  // Put the viewer below the menubar. pack_start "grows" the widget
  // by default, so it'll take up the rest of the window.
  m_viewer.set_size_request(300, 300);
  m_vbox.pack_start(m_viewer);

  Gtk::Label label("some label");
  label.set_text(m_viewer.m_label);

  m_vbox.pack_start(label);

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
    m_viewer.reset();
  } else if (ev->keyval == 'o' || ev->keyval == 'O') {
    m_viewer.set_mode(m_viewer.VIEW_ROTATE);
  } else if (ev->keyval == 'n' || ev->keyval == 'N') {
    m_viewer.set_mode(m_viewer.VIEW_TRANSLATE);
  } else if (ev->keyval == 'p' || ev->keyval == 'P') {
    m_viewer.set_mode(m_viewer.VIEW_PERSPECTIVE);
  } else if (ev->keyval == 'r' || ev->keyval == 'R') {
    m_viewer.set_mode(m_viewer.MODEL_ROTATE);
  } else if (ev->keyval == 's' || ev->keyval == 'S') {
    m_viewer.set_mode(m_viewer.MODEL_SCALE);
  } else if (ev->keyval == 't' || ev->keyval == 'T') {
    m_viewer.set_mode(m_viewer.MODEL_TRANSLATE);
  } else if (ev->keyval == 'v' || ev->keyval == 'V') {
    m_viewer.set_mode(m_viewer.VIEWPORT);
  }

  //return true;
  return Gtk::Window::on_key_press_event( ev );
}

