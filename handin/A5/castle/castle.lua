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

-- filenames for texs
castle_wall_fn = "assets/castle_wall_texture_3.bmp";
--sky_fn = "assets/sky_texture_1.png";

-- materials
red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 1.0, -1);
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 1.0, -1 );
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 1.0, -1);
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 1.0, -1);
black = gr.material({0.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 1.0, 1);
grass_c = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 1.0, 1);
sky_c = gr.material({0.2, 0.4, 1.0}, {0.1, 0.1, 0.1}, 1.0, 1);
castle_wall_c = gr.material({0.6, 0.5, 0.5}, {0.1, 0.1, 0.1}, 1.0, 1);
crystal_c = gr.material({0.7, 0.3, 0.6}, {0.1, 0.1, 0.1}, 0.4, -1);
crystal2_c = gr.material({0.3, 0.4, 0.7}, {0.1, 0.1, 0.1}, 0.5, -1);

-- textures
-- tex_type, tex_id, tex_w, tex_h, tex_filename
none_t = gr.texture(tex_none, 0, 0, 0, "", 0, 0);
castle_wall_t = gr.texture(tex_image, texid_cw, 256, 256, castle_wall_fn, 0, 0);
--sky_t = gr.texture(tex_image, texid_sky, 900, 600, sky_fn, 0, 0);
sky_t = gr.texture(tex_perlin, texid_sky, 64, 64, "", 4, 2);
grass_t = gr.texture(tex_perlin, texid_grass, 64, 64, "", 8, 2);

---- rootnode ----
rootnode = gr.node('rootnode');

  -- sky terrain --
  sky_w = 140
  sky_h = 60
  sky_b = 1

--[[
  sky_node = gr.node('sky_node');
  sky_node:translate(-sky_w/2, -ground_h/2, 0);
  rootnode:add_child(sky_node);

  sky_primt = gr.cube('sky_primt');
  sky_primt:scale(sky_w, sky_h, sky_b);
  sky_primt:set_texture(sky_t);
  sky_primt:set_material(sky_c);
  sky_node:add_child(sky_primt);
]]

  model_node = gr.node('model_node');
  rootnode:add_child(model_node);

  -- terrain --
  terrain_w = 25
  terrain_h = 3
  terrain_b = 15
  terrain_primt = gr.model('terrain_primt');
  terrain_primt:translate(0, -terrain_h, 0);
  terrain_primt:scale(terrain_w, terrain_h, terrain_b);
  terrain_primt:set_texture(grass_t);
  terrain_primt:set_material(grass_c);
  terrain_primt:celshading();
  terrain_primt:set_filename("models/terrain.obj");
  model_node:add_child(terrain_primt);

  -- castle --
  castle_w = 6
  castle_h = 6
  castle_b = 4
  castle_primt = gr.model('castle_primt');
  castle_primt:scale(castle_w, castle_h, castle_b);
  castle_primt:set_texture(castle_wall_t);
  castle_primt:set_material(castle_wall_c);
  castle_primt:celshading();
  castle_primt:set_filename("models/castle2/castle.obj");
  model_node:add_child(castle_primt);

  -- crystals --
  crystal_w = 2
  crystal_h = 2
  crystal_b = 2
  crystal_primt = gr.model('crystal_primt');
  crystal_primt:rotate('x', 90.0);
  crystal_primt:translate(-10, 0, 5);
  crystal_primt:scale(crystal_w, crystal_h, crystal_b);
  crystal_primt:set_material(crystal_c);
  crystal_primt:set_filename("models/crystal2/crystal.obj");
  model_node:add_child(crystal_primt);

  crystal_w = 3
  crystal_h = 3
  crystal_b = 3
  crystal_primt = gr.model('crystal_primt');
  crystal_primt:rotate('x', 90.0);
  crystal_primt:rotate('y', 3.0);
  crystal_primt:rotate('z', 3.0);
  crystal_primt:translate(10, 0, 5);
  crystal_primt:scale(crystal_w, crystal_h, crystal_b);
  crystal_primt:set_material(crystal2_c);
  crystal_primt:set_filename("models/crystal2/crystal.obj");
  model_node:add_child(crystal_primt);

-- main hall --
-- centecastle_wall_c on screen, cube that makes up body of castle --
--[[
mh_w = 2
mh_h = 2
mh_b = 2
mh_thickness = 1;

  -- back wall --
  mh_b_node = gr.node('mh_b_node');
  mh_b_node:translate(-mh_w*2, 0, 1);
  rootnode:add_child(mh_b_node);

  mh_b_primt = gr.cube('mh_b_primt');
  mh_b_primt:scale(mh_w, mh_h, mh_thickness);
  mh_b_primt:celshading();
  mh_b_primt:set_texture(castle_wall_t);
  mh_b_primt:set_material(castle_wall_c);
  mh_b_node:add_child(mh_b_primt);
]]
--rootnode:translate(0.0, 0, 20.0)
--rootnode:scale(5, 5, 5);
--rootnode:rotate('y', -20.0)
return rootnode
