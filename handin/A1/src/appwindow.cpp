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
  m_menu_app.items().push_back(MenuElem("_Quit", Gtk::AccelKey("Q"),
    sigc::mem_fun(*this, &AppWindow::hide)));
  m_menu_app.items().push_back(MenuElem("_New game", Gtk::AccelKey("N"),
    sigc::mem_fun(m_viewer, &Viewer::new_game)));

  // Set up the draw mode radio menu group
  sigc::slot1<void, Viewer::Mode> mode_slot = sigc::mem_fun(m_viewer, &Viewer::set_mode);
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_Wire-frame", Gtk::AccelKey("W"),
    sigc::bind(mode_slot, Viewer::WIREFRAME)));
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_Face", Gtk::AccelKey("F"),
    sigc::bind(mode_slot, Viewer::FACE)));
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_Multicoloured", Gtk::AccelKey("M"),
    sigc::bind(mode_slot, Viewer::MULTICOLOURED)));

  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Draw Mode", m_menu_mode));

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

bool AppWindow::on_key_press_event( GdkEventKey *ev )
{
        // This is a convenient place to handle non-shortcut
        // keys.  You'll want to look at ev->keyval.

	// An example key; delete and replace with the
	// keys you want to process
        if( ev->keyval == 't' ) {
                std::cerr << "Hello!" << std::endl;
                return true;
        } else {
                return Gtk::Window::on_key_press_event( ev );
        }
}
