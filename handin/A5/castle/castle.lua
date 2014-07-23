--
-- CS488 -- Introduction to Computer Graphics
--
-- puppet.lua
--

--[[
s2 = gr.cube('s2')
rootnode:add_child(s2)
s2:translate(2.0, -2.0, 0.0)
s2:rotate('z', -90.0)
s2:scale(0.1, 2.0, 0.1)
s2:set_material(blue)

s3 = gr.cube('s3')
rootnode:add_child(s3)
s3:scale(0.1, 0.1, 2.0)
s3:translate(0.0, -20.0, 1.0)
s3:set_material(green)
]]

-- corresponds to Mode enum in material.hpp
tex_image = 0
tex_perlin = 1
tex_none = 2

-- id for texs
texid_cw = 0
texid_grass = 1
texid_sky = 2
texid_tree = 3

-- filenames for texs
castle_wall_fn = "assets/castle_wall_texture_1.bmp";
sky_fn = "assets/sky.bmp";
tree_fn = "assets/crystal2.bmp";
guard_fn = "assets/crystal1.bmp";
guard_fn2 = "assets/crystal7.bmp";
crystal_fn = "assets/crystal4.bmp";
crystal_fn2 = "assets/crystal5.bmp";

-- materials
red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 1.0, -1);
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 1.0, -1 );
green = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 1.0, -1);
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 1.0, -1);
black = gr.material({0.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 1.0, 1);
grass_c = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 1.0, 1);
sky_c = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 1.0, 1);
castle_wall_c = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 1.0, 1);
crystal_c = gr.material({0.7, 0.3, 0.6}, {0.1, 0.1, 0.1}, 0.5, -1);
crystal2_c = gr.material({0.3, 0.4, 0.7}, {0.1, 0.1, 0.1}, 0.6, -1);
tree_c = gr.material({0.2, 0.7, 0.2}, {0.1, 0.1, 0.1}, 1.0, -1);
tree_c2 = gr.material({0.0, 0.5, 0.0}, {0.1, 0.1, 0.1}, 1.0, -1);
tree_c3 = gr.material({0.1, 0.6, 0.1}, {0.1, 0.1, 0.1}, 1.0, -1);
guard_c = gr.material({0.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 1.0, -1);

-- textures
-- tex_type, tex_id, tex_w, tex_h, tex_filename
none_t = gr.texture(tex_none, 0, 0, 0, "", 0, 0);
castle_wall_t = gr.texture(tex_image, texid_cw, 256, 256, castle_wall_fn, 0, 0);
sky_t = gr.texture(tex_image, texid_sky, 1000, 750, sky_fn, 0, 0);
tree_t = gr.texture(tex_image, texid_tree, 300, 300, tree_fn, 0, 0);
guard_t = gr.texture(tex_image, texid_tree, 300, 300, guard_fn, 0, 0);
guard_t2 = gr.texture(tex_image, texid_tree, 300, 300, guard_fn2, 0, 0);
crystal_t = gr.texture(tex_image, texid_tree, 300, 300, crystal_fn, 0, 0);
crystal_t2 = gr.texture(tex_image, texid_tree, 300, 300, crystal_fn2, 0, 0);
grass_t = gr.texture(tex_perlin, texid_grass, 64, 64, "", 16, 2);
sky_perlin_t = gr.texture(tex_perlin, texid_sky, 64, 64, "", 16, 2);

---- rootnode ----
rootnode = gr.node('rootnode');

  -- sky terrain --
  sky_w = 100
  sky_h = 100
  sky_b = 100

  sky_node = gr.node('sky_node');
  sky_node:translate(-sky_w/2, -10, -sky_b/2);
  rootnode:add_child(sky_node);

  sky_primt = gr.cube('sky_primt');
  sky_primt:scale(sky_w, sky_h, sky_b);
  sky_primt:set_texture(sky_t);
  sky_primt:set_material(sky_c);
  sky_node:add_child(sky_primt);

  model_node = gr.node('model_node');
  rootnode:add_child(model_node);

  -- terrain --
  terrain_w = 24
  terrain_h = 3
  terrain_b = 20
  terrain_primt = gr.model('terrain_primt');
  terrain_primt:translate(0, -5 - terrain_h, 0);
  terrain_primt:scale(terrain_w, terrain_h, terrain_b);
  terrain_primt:set_texture(grass_t);
  terrain_primt:set_material(grass_c);
  terrain_primt:celshading();
  terrain_primt:set_filename("models/terrain.obj");
  model_node:add_child(terrain_primt);

  -- crystals --
  crystal_w = 2
  crystal_h = 2
  crystal_b = 2
  crystal_node = gr.node('crystal_node');
  crystal_node:translate(0, -6, 0);
  model_node:add_child(crystal_node);

  crystal_primt = gr.model('crystal_primt');
  crystal_primt:rotate('x', 90.0);
  crystal_primt:translate(-10, 0, 0);
  crystal_primt:scale(crystal_w, crystal_h, crystal_b);
  crystal_primt:set_material(crystal_c);
  crystal_primt:set_texture(crystal_t2);
  crystal_primt:celshading();
  crystal_primt:set_filename("models/crystal2/crystal.obj");
  crystal_node:add_child(crystal_primt);

  crystal_w = 3
  crystal_h = 3
  crystal_b = 3
  crystal_primt = gr.model('crystal_primt');
  crystal_primt:rotate('x', 90.0);
  crystal_primt:rotate('y', 3.0);
  crystal_primt:rotate('z', 3.0);
  crystal_primt:translate(10, 0, 0);
  crystal_primt:scale(crystal_w, crystal_h, crystal_b);
  crystal_primt:set_material(crystal2_c);
  crystal_primt:celshading();
  crystal_primt:set_texture(crystal_t);
  crystal_primt:set_filename("models/crystal2/crystal.obj");
  crystal_node:add_child(crystal_primt);

  -- castle --
  castle_w = 6
  castle_h = 6
  castle_b = 6

  castle_node = gr.node('castle_node');
  castle_node:translate(0, -castle_h, 0);
  model_node:add_child(castle_node);

  castle_primt = gr.model('castle_primt');
  castle_primt:scale(castle_w, castle_h, castle_b);
  castle_primt:rotate('y', -90.0);
  castle_primt:set_texture(castle_wall_t);
  castle_primt:set_material(castle_wall_c);
  castle_primt:celshading();
  castle_primt:set_filename("models/castle2/castle.obj");
  castle_node:add_child(castle_primt);

  -- guards --
  guard_w = 1
  guard_h = 1
  guard_b = 1
  guard_node = gr.node('guard_node');
  guard_node:translate(0, -7, 0);
  model_node:add_child(guard_node);

  guard_primt = gr.model('guard_primt');
  guard_primt:translate(-3, 0, 8);
  guard_primt:scale(guard_w, guard_h, guard_b);
  --guard_primt:rotate('y', 180.0);
  guard_primt:set_material(guard_c);
  guard_primt:celshading();
  guard_primt:set_texture(guard_t);
  guard_primt:set_filename("models/bot2/zolabot.obj");
  guard_node:add_child(guard_primt);

  guard_primt = gr.model('guard_primt');
  guard_primt:translate(3, 0, 8);
  guard_primt:scale(guard_w, guard_h, guard_b);
  --guard_primt:rotate('y', 180.0);
  guard_primt:set_material(guard_c);
  guard_primt:celshading();
  guard_primt:set_texture(guard_t2);
  guard_primt:set_filename("models/bot2/zolabot.obj");
  guard_node:add_child(guard_primt);

  -- trees --
  tree_w = 5.0
  tree_h = 5.0
  tree_b = 5.0
  tree_node = gr.node('tree_node');
  tree_node:translate(0, -10, 0);
  model_node:add_child(tree_node);

  -- left
  tree_primt = gr.model('tree_primt');
  tree_primt:scale(tree_w, tree_h, tree_b);
  tree_primt:translate(-2.5, 0, -1);
  tree_primt:rotate('y', 90.0);
  tree_primt:celshading();
  tree_primt:set_material(tree_c2);
  tree_primt:set_texture(tree_t);
  tree_primt:set_filename("models/trees.obj");
  tree_node:add_child(tree_primt);

  -- right
  tree_primt = gr.model('tree_primt');
  tree_primt:translate(9, 0, -3);
  tree_primt:scale(tree_w, tree_h, tree_b);
  tree_primt:rotate('y', -90.0);
  tree_primt:celshading();
  tree_primt:set_material(tree_c3);
  tree_primt:set_texture(tree_t);
  tree_primt:set_filename("models/trees.obj");
  tree_node:add_child(tree_primt);

  -- front
  tree_primt = gr.model('tree_primt');
  tree_primt:translate(0, 0, 0);
  tree_primt:scale(tree_w, tree_h, tree_b);
  tree_primt:rotate('y', 180.0);
  tree_primt:celshading();
  tree_primt:set_material(tree_c);
  tree_primt:set_texture(tree_t);
  tree_primt:set_filename("models/trees.obj");
  tree_node:add_child(tree_primt);

rootnode:translate(0.0, 3.0, 0.0)
--rootnode:scale(0.7, 0.7, 0.7);
rootnode:rotate('x', -30.0)
return rootnode
