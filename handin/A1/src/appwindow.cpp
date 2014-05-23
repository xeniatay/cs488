#include "appwindow.hpp"
#include <iostream>

AppWindow::AppWindow()
{
  set_title("488 Tetrominoes on the Wall");

  // A utility class for constructing things that go into menus, which
  // we'll set up next.
  using Gtk::Menu_Helpers::MenuElem;
  using Gtk::Menu_Helpers::RadioMenuElem;

  // Set up the application menu
  // The slot we use here just causes AppWindow::hide() on this,
  // which shuts down the application.
  m_menu_app.items().push_back(MenuElem("_Quit (Q)", sigc::mem_fun(*this, &AppWindow::hide)));
  m_menu_app.items().push_back(MenuElem("_New game (N)", sigc::mem_fun(m_viewer, &Viewer::new_game)));
  m_menu_app.items().push_back(MenuElem("_Reset (R)", sigc::mem_fun(m_viewer, &Viewer::reset)));

  // Set up the draw mode radio menu group
  sigc::slot1<void, Viewer::Mode> mode_slot = sigc::mem_fun(m_viewer, &Viewer::set_mode);
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_Wire-frame (W)", sigc::bind(mode_slot, Viewer::WIREFRAME)));
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_Face (F)", sigc::bind(mode_slot, Viewer::FACE)));
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_Multicoloured (M)", sigc::bind(mode_slot, Viewer::MULTICOLOURED)));

  // Set up the speed menu
  sigc::slot1<void, Viewer::Speed> speed_slot = sigc::mem_fun(m_viewer, &Viewer::set_speed);
  m_menu_speed.items().push_back(RadioMenuElem(m_menu_speed_group, "_Slow", Gtk::AccelKey("1"),
    sigc::bind(speed_slot, Viewer::SLOW)));
  m_menu_speed.items().push_back(RadioMenuElem(m_menu_speed_group, "_Medium", Gtk::AccelKey("2"),
    sigc::bind(speed_slot, Viewer::MEDIUM)));
  m_menu_speed.items().push_back(RadioMenuElem(m_menu_speed_group, "_Fast", Gtk::AccelKey("3"),
    sigc::bind(speed_slot, Viewer::FAST)));

  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Draw Mode", m_menu_mode));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Speed", m_menu_speed));

  // Pack in our widgets

  // First add the vertical box as our single "top" widget
  add(m_vbox);

  // Put the menubar on the top, and make it as small as possible
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);

  // Put the viewer below the menubar. pack_start "grows" the widget
  // by default, so it'll take up the rest of the window.
  m_viewer.set_size_request(300, 600);

  m_vbox.pack_start(m_viewer);

  show_all();

}

bool AppWindow::on_key_press_event( GdkEventKey *ev ) {
  // This is a convenient place to handle non-shortcut
  // keys.  You'll want to look at ev->keyval.

	// An example key; delete and replace with the
	// keys you want to process
  //std::cerr << "Key Value: " << ev->keyval << std::endl;

  if( ev->keyval == 65361 && !m_viewer.game_over) {
    m_viewer.press_left();
  } else if( ev->keyval == 65362 && !m_viewer.game_over) {
    m_viewer.press_up();
  } else if( ev->keyval == 65363 && !m_viewer.game_over) {
    m_viewer.press_right();
  } else if( ev->keyval == 65364 && !m_viewer.game_over) {
    m_viewer.press_down();
  } else if (ev->keyval == 32 && !m_viewer.game_over) {
    std::cerr << "Spacebar" << std::endl;
    m_viewer.press_space();
  } else if (ev->keyval == 'q' || ev->keyval == 'Q') {
    hide();
  } else if (ev->keyval == 'n' || ev->keyval == 'N') {
    m_viewer.new_game();
  } else if (ev->keyval == 'r' || ev->keyval == 'R') {
    m_viewer.reset();
  } else if (ev->keyval == 'w' || ev->keyval == 'W') {
    m_viewer.set_mode(m_viewer.WIREFRAME);
  } else if (ev->keyval == 'f' || ev->keyval == 'F') {
    m_viewer.set_mode(m_viewer.FACE);
  } else if (ev->keyval == 'm' || ev->keyval == 'M') {
    m_viewer.set_mode(m_viewer.MULTICOLOURED);
  }
  //return true;
  return Gtk::Window::on_key_press_event( ev );
}
