// Xenia Tay
// 20396769
// xzytay

#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <gtkmm.h>
#include "viewer.hpp"

class AppWindow : public Gtk::Window {
public:
  AppWindow();

protected:
  virtual bool on_key_press_event( GdkEventKey *ev );

private:
  // A "vertical box" which holds everything in our window
  Gtk::VBox m_vbox;

  // The menubar, with all the menus at the top of the window
  Gtk::MenuBar m_menubar;
  // Each menu itself
  Gtk::Menu m_menu_app;
  Gtk::Menu m_menu_mode;

  // Draw Mode Radio Button Menu
  Gtk::RadioButtonGroup m_menu_mode_group;

  // The main OpenGL area
  Viewer m_viewer;
};

#endif
