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
  m_menu_app.items().push_back(MenuElem("_Reset Position (I)", sigc::mem_fun(m_viewer, &Viewer::reset_position)));
  m_menu_app.items().push_back(MenuElem("_Reset Orientation (O)", sigc::mem_fun(m_viewer, &Viewer::reset_orientation)));
  m_menu_app.items().push_back(MenuElem("_Reset Joints (N)", sigc::mem_fun(m_viewer, &Viewer::reset_joints)));
  m_menu_app.items().push_back(MenuElem("_Reset All (A)", sigc::mem_fun(m_viewer, &Viewer::reset_all)));
  m_menu_app.items().push_back(MenuElem("_Quit (Q)", sigc::mem_fun(*this, &AppWindow::hide)));

  // Set up the mode radio menu group
  sigc::slot1<void, Viewer::Mode> mode_slot = sigc::mem_fun(m_viewer, &Viewer::set_mode);
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_Position/Orientation (P)", sigc::bind(mode_slot, Viewer::POSITION_OR_ORIENTATION)));
  m_menu_mode.items().push_back(RadioMenuElem(m_menu_mode_group, "_Joints (J)", sigc::bind(mode_slot, Viewer::JOINTS)));

  // Set up the edit menu
  m_menu_edit.items().push_back(MenuElem("_Undo (U)", sigc::mem_fun(m_viewer, &Viewer::undo)));
  m_menu_edit.items().push_back(MenuElem("_Redo (R)", sigc::mem_fun(m_viewer, &Viewer::redo)));

  // Set up the options checklist menu group
  sigc::slot1<void, Viewer::Option> option_slot = sigc::mem_fun(m_viewer, &Viewer::set_options);
  m_menu_options.items().push_back(CheckMenuElem("_Circle (C)", sigc::bind(option_slot, Viewer::CIRCLE)));
  m_menu_options.items().push_back(CheckMenuElem("_Z-buffer (Z)", sigc::bind(option_slot, Viewer::Z_BUFFER)));
  m_menu_options.items().push_back(CheckMenuElem("_Backface Cull (B)", sigc::bind(option_slot, Viewer::BACKFACE_CULL)));
  m_menu_options.items().push_back(CheckMenuElem("_Frontface Cull (F)", sigc::bind(option_slot, Viewer::FRONTFACE_CULL)));

  // Set up the options checklist menu group
  sigc::slot1<void, Viewer::Picking> picking_slot = sigc::mem_fun(m_viewer, &Viewer::set_pickings);
  m_menu_pickings.items().push_back(CheckMenuElem("_Left Upper Arm", sigc::bind(picking_slot, Viewer::LEFT_UPPER_ARM)));
  m_menu_pickings.items().push_back(CheckMenuElem("_Left Forearm", sigc::bind(picking_slot, Viewer::LEFT_FOREARM)));
  m_menu_pickings.items().push_back(CheckMenuElem("_Left Hand", sigc::bind(picking_slot, Viewer::LEFT_HAND)));
  m_menu_pickings.items().push_back(CheckMenuElem("_Right Upper Arm", sigc::bind(picking_slot, Viewer::RIGHT_UPPER_ARM)));
  m_menu_pickings.items().push_back(CheckMenuElem("_Right Forearm", sigc::bind(picking_slot, Viewer::RIGHT_FOREARM)));
  m_menu_pickings.items().push_back(CheckMenuElem("_Right Hand", sigc::bind(picking_slot, Viewer::RIGHT_HAND)));
  m_menu_pickings.items().push_back(CheckMenuElem("_Neck", sigc::bind(picking_slot, Viewer::NECK)));
  m_menu_pickings.items().push_back(CheckMenuElem("_Head", sigc::bind(picking_slot, Viewer::HEAD)));
  m_menu_pickings.items().push_back(CheckMenuElem("_Left Thigh", sigc::bind(picking_slot, Viewer::LEFT_THIGH)));
  m_menu_pickings.items().push_back(CheckMenuElem("_Left Calf", sigc::bind(picking_slot, Viewer::LEFT_CALF)));
  m_menu_pickings.items().push_back(CheckMenuElem("_Left Foot", sigc::bind(picking_slot, Viewer::LEFT_FOOT)));
  m_menu_pickings.items().push_back(CheckMenuElem("_Right Thigh", sigc::bind(picking_slot, Viewer::RIGHT_THIGH)));
  m_menu_pickings.items().push_back(CheckMenuElem("_Right Calf", sigc::bind(picking_slot, Viewer::RIGHT_CALF)));
  m_menu_pickings.items().push_back(CheckMenuElem("_Right Foot", sigc::bind(picking_slot, Viewer::RIGHT_FOOT)));

  // Set up the menu bar
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Application", m_menu_app));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Mode", m_menu_mode));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Edit", m_menu_edit));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Options", m_menu_options));
  m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Picking", m_menu_pickings));

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
  } else if (ev->keyval == 'i' || ev->keyval == 'I') {
    m_viewer.reset_position();
  } else if (ev->keyval == 'o' || ev->keyval == 'O') {
    m_viewer.reset_orientation();
  } else if (ev->keyval == 'n' || ev->keyval == 'N') {
    m_viewer.reset_joints();
  } else if (ev->keyval == 'a' || ev->keyval == 'A') {
    m_viewer.reset_all();
  } else if (ev->keyval == 'p' || ev->keyval == 'P') {
    m_viewer.set_mode(m_viewer.POSITION_OR_ORIENTATION);
  } else if (ev->keyval == 'j' || ev->keyval == 'J') {
    m_viewer.set_mode(m_viewer.JOINTS);
  } else if (ev->keyval == 'u' || ev->keyval == 'U') {
    m_viewer.undo();
  } else if (ev->keyval == 'r' || ev->keyval == 'R') {
    m_viewer.redo();
  } else if (ev->keyval == 'c' || ev->keyval == 'C') {
    m_viewer.set_options(m_viewer.CIRCLE);
  } else if (ev->keyval == 'z' || ev->keyval == 'Z') {
    m_viewer.set_options(m_viewer.Z_BUFFER);
  } else if (ev->keyval == 'b' || ev->keyval == 'B') {
    m_viewer.set_options(m_viewer.BACKFACE_CULL);
  } else if (ev->keyval == 'f' || ev->keyval == 'F') {
    m_viewer.set_options(m_viewer.FRONTFACE_CULL);
  }

  //return true;
  return Gtk::Window::on_key_press_event( ev );
}
