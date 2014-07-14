#include <iostream>
#include <gtkmm.h>
#include <gtkglmm.h>
#include "appwindow.hpp"
#include "scene_lua.hpp"
#include "image.hpp"

int main(int argc, char** argv)
{
  // Construct our main loop
  Gtk::Main kit(argc, argv);

  // Initialize OpenGL
  Gtk::GL::init(argc, argv);

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

  Image img(300, 300, 3);
  img.loadPng("assets/castle_wall_texture.png");
  img.savePng("test.png");

}

