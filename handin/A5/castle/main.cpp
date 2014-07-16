#include <iostream>
#include <gtkmm.h>
#include <gtkglmm.h>
#include "appwindow.hpp"
#include "scene_lua.hpp"

int main(int argc, char** argv)
{
  // Construct our main loop
  Gtk::Main kit(argc, argv);

  // Initialize OpenGL
  Gtk::GL::init(argc, argv);

  char *myargv [1];
  int myargc = 1;
  myargv[0] = "ct";

  std::string filename = "castle.lua";
  if (argc >= 2) {
    filename = argv[1];
  }
  // This is how you might import a scene.
  SceneNode* root = import_lua(filename);
  if (!root) {
    std::cerr << "Could not open " << filename << std::endl;
    return 1;
  }

  // Construct our (only) window
  AppWindow window;

  // And run the application!
  Gtk::Main::run(window);

}

