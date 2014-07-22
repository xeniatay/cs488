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
castle_wall_fn = "assets/castle_wall_texture_3.png";
sky_fn = "assets/sky_texture_1.png";

-- materials
red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 1.0, -1);
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 1.0, -1 );
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 1.0, -1);
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 1.0, -1);
black = gr.material({0.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 1.0, 1);
grass_c = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 1.0, 1);
sky_c = gr.material({0.2, 0.4, 1.0}, {0.1, 0.1, 0.1}, 1.0, 1);
castle_wall_c = gr.material({0.6, 0.5, 0.5}, {0.1, 0.1, 0.1}, 1.0, 1);
castle_wall_window_c = gr.material({0.8, 1.0, 1.0}, {0.1, 0.1, 0.1}, 0.1, -1);

-- textures
-- tex_type, tex_id, tex_w, tex_h, tex_filename
none_t = gr.texture(tex_none, 0, 0, 0, "", 0, 0);
castle_wall_t = gr.texture(tex_image, texid_cw, 300, 300, castle_wall_fn, 0, 0);
--sky_t = gr.texture(tex_image, texid_sky, 900, 600, sky_fn, 0, 0);
sky_t = gr.texture(tex_perlin, texid_sky, 64, 64, "", 4, 2);
grass_t = gr.texture(tex_perlin, texid_grass, 64, 64, "", 16, 1);

---- rootnode ----
rootnode = gr.node('rootnode');

--[[

  -- sky terrain --
  sky_w = 140
  sky_h = 60
  sky_b = 1
  sky_node = gr.node('sky_node');
  sky_node:translate(-sky_w/2, 0, 0);
  rootnode:add_child(sky_node);

  sky_primt = gr.cube('sky_primt');
  sky_primt:scale(sky_w, sky_h, sky_b);
  sky_primt:set_texture(sky_t);
  sky_primt:set_material(sky_c);
  sky_node:add_child(sky_primt);


  -- ground terrain --
  ground_w = 140
  ground_h = 1
  ground_b = 65
  ground_node = gr.node('ground_node');
  ground_node:translate(-ground_w/2, 0, 0);
  rootnode:add_child(ground_node);

  ground_primt = gr.cube('ground_primt');
  ground_primt:scale(ground_w, ground_h, ground_b);
  ground_primt:set_texture(grass_t);
  ground_primt:set_material(grass_c);
  ground_node:add_child(ground_primt);


-- main hall --
-- centecastle_wall_c on screen, cube that makes up body of castle --
mh_w = 30
mh_h = 10
mh_b = 10
mh_thickness = 1;

  -- back wall --
  mh_b_node = gr.node('mh_b_node');
  mh_b_node:translate(-mh_w/2, 0, 1);
  rootnode:add_child(mh_b_node);

  mh_b_primt = gr.cube('mh_b_primt');
  mh_b_primt:dimensions(mh_w, mh_h, mh_thickness);
  mh_b_primt:celshading();
  mh_b_primt:set_texture(castle_wall_t);
  mh_b_primt:set_material(castle_wall_c);
  mh_b_node:add_child(mh_b_primt);

  -- left wall --
  mh_l_node = gr.node('mh_l_node');
  mh_l_node:translate(-mh_w/2, 0, 1);
  rootnode:add_child(mh_l_node);

  mh_l_primt = gr.cube('mh_l_primt');
  mh_l_primt:dimensions(1, mh_h, mh_b);
  mh_l_primt:celshading();
  mh_l_primt:set_texture(castle_wall_t);
  mh_l_primt:set_material(castle_wall_c);
  mh_l_node:add_child(mh_l_primt);

  -- right wall --
  mh_r_node = gr.node('mh_r_node');
  mh_r_node:translate(mh_w/2 - mh_thickness, 0, 1);
  rootnode:add_child(mh_r_node);

  mh_r_primt = gr.cube('mh_r_primt');
  mh_r_primt:dimensions(1, mh_h, mh_b);
  mh_r_primt:celshading();
  mh_r_primt:set_texture(castle_wall_t);
  mh_r_primt:set_material(castle_wall_c);
  mh_r_node:add_child(mh_r_primt);

  -- front wall --
  mh_f_node = gr.node('mh_f_node');
  mh_f_node:translate(-mh_w/2, 0, mh_b);
  rootnode:add_child(mh_f_node);

  -- front wall left --
  mh_fl_primt = gr.cube('mh_fl_primt');
  mh_fl_primt:dimensions(mh_w/3, mh_h, mh_thickness);
  mh_fl_primt:celshading();
  mh_fl_primt:set_texture(castle_wall_t);
  mh_fl_primt:set_material(castle_wall_c);
  mh_f_node:add_child(mh_fl_primt);

  -- front wall middle bottom --
  mh_fmb_primt = gr.cube('mh_fmb_primt');
  mh_fmb_primt:translate(mh_w/3, 0, 0);
  mh_fmb_primt:dimensions(mh_w/3, mh_h/3, mh_thickness);
  mh_fmb_primt:celshading();
  mh_fmb_primt:set_texture(castle_wall_t);
  mh_fmb_primt:set_material(castle_wall_c);
  mh_f_node:add_child(mh_fmb_primt);

  -- front wall window --
  mh_fw_primt = gr.cube('mh_fw_primt');
  mh_fw_primt:translate(mh_w/3, mh_h/3, 0);
  mh_fw_primt:scale(mh_w/3, mh_h/3, mh_thickness);
  mh_fw_primt:set_material(castle_wall_window_c);
  mh_f_node:add_child(mh_fw_primt);

  -- front wall middle top --
  mh_fmt_primt = gr.cube('mh_fmt_primt');
  mh_fmt_primt:translate(mh_w/3, 2 * mh_h/3, 0);
  mh_fmt_primt:dimensions(mh_w/3, mh_h/3, mh_thickness);
  mh_fmt_primt:celshading();
  mh_fmt_primt:set_texture(castle_wall_t);
  mh_fmt_primt:set_material(castle_wall_c);
  mh_f_node:add_child(mh_fmt_primt);

  -- front wall right --
  mh_fr_primt = gr.cube('mh_fr_primt');
  mh_fr_primt:translate(2 * mh_w/3, 0, 0);
  mh_fr_primt:dimensions(mh_w/3, mh_h, mh_thickness);
  mh_fr_primt:celshading();
  mh_fr_primt:set_texture(castle_wall_t);
  mh_fr_primt:set_material(castle_wall_c);
  mh_f_node:add_child(mh_fr_primt);

-- end main hall --

-- start castle battlements --

battlement_w = mh_w / 11;
battlement_h = 2;
battlement_thickness = mh_thickness;

  -- front and back battlements --
  num_battlements = math.ceil(mh_w / battlement_w / 2);
  for i = 0, (num_battlements - 1 ) do

    -- generate battlements
    battlement_node = gr.node("battlement_node_" .. i );
    battlement_node:translate(i * battlement_w * 2, mh_h, 0);
    mh_f_node:add_child(battlement_node)
    mh_b_node:add_child(battlement_node)

    battlement_primt = gr.cube("battlement_primt_" .. i);
    battlement_primt:dimensions(battlement_w, battlement_h, battlement_thickness);
    battlement_primt:set_texture(castle_wall_t);
    battlement_primt:set_material(castle_wall_c)
    battlement_node:add_child(battlement_primt)

  end

  -- left and right battlements --
  num_battlements = math.ceil(mh_b / battlement_w / 2);
  for i = 0, (num_battlements - 1 ) do

    -- generate battlements
    battlement_node = gr.node("battlement_node_" .. i );
    battlement_node:translate(0, mh_h, i * battlement_w * 2);
    mh_l_node:add_child(battlement_node)
    mh_r_node:add_child(battlement_node)

    battlement_primt = gr.cube("battlement_primt_" .. i);
    battlement_primt:dimensions(battlement_thickness, battlement_h, battlement_w);
    battlement_primt:set_texture(castle_wall_t);
    battlement_primt:set_material(castle_wall_c)
    battlement_node:add_child(battlement_primt)

  end

  sphere_primt = gr.sphere('sphere_primt');
  sphere_primt:translate(-30, 40, 60);
  sphere_primt:scale(10, 10, 10);
  sphere_primt:set_material(red);
  rootnode:add_child(sphere_primt);

]]
-- end castle battlements --
--rootnode:translate(8.0, 0, 0.0)
rootnode:scale(0.5, 0.5, 0.5);
--rootnode:rotate('y', -20.0)
return rootnode
