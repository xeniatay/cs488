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

-- materials
red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10);
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10);
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10);
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10);
black = gr.material({0.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10);

---- rootnode ----
rootnode = gr.node('rootnode');

-- main hall --
-- centered on screen, cube that makes up body of castle --
mh_w = 40
mh_h = 30
mh_b = 20
mh_thickness = 1;

  -- back wall --
  mh_b_node = gr.node('mh_b_node');
  mh_b_node:translate(-mh_w/2, -mh_h/2, 1);
  rootnode:add_child(mh_b_node);

  mh_b_primt = gr.cube('mh_b_primt');
  mh_b_primt:scale(mh_w, mh_h, mh_thickness);
  mh_b_primt:set_material(red);
  mh_b_node:add_child(mh_b_primt);

  -- left wall --
  mh_l_node = gr.node('mh_l_node');
  mh_l_node:translate(-mh_w/2, -mh_h/2, 1);
  rootnode:add_child(mh_l_node);

  mh_l_primt = gr.cube('mh_l_primt');
  mh_l_primt:scale(1, mh_h, mh_b);
  mh_l_primt:set_material(green);
  mh_l_node:add_child(mh_l_primt);

  -- right wall --
  mh_r_node = gr.node('mh_r_node');
  mh_r_node:translate(mh_w/2 - mh_thickness, -mh_h/2, 1);
  rootnode:add_child(mh_r_node);

  mh_r_primt = gr.cube('mh_r_primt');
  mh_r_primt:scale(1, mh_h, mh_b);
  mh_r_primt:set_material(green);
  mh_r_node:add_child(mh_r_primt);

  -- front wall --
  mh_f_node = gr.node('mh_f_node');
  mh_f_node:translate(-mh_w/2, -mh_h/2, mh_b);
  rootnode:add_child(mh_f_node);

  mh_f_primt = gr.cube('mh_f_primt');
  mh_f_primt:scale(mh_w, mh_h, mh_thickness);
  mh_f_primt:set_material(blue);
  mh_f_node:add_child(mh_f_primt);

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
    battlement_primt:scale(battlement_w, battlement_h, battlement_thickness);
    battlement_primt:set_material(black)
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
    battlement_primt:scale(battlement_thickness, battlement_h, battlement_w);
    battlement_primt:set_material(black)
    battlement_node:add_child(battlement_primt)

  end

-- end castle battlements --

--rootnode:translate(0, 0, 5.0)
rootnode:scale(0.2, 0.2, 0.2);
--rootnode:rotate('y', -20.0)

return rootnode
