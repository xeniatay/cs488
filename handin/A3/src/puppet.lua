--
-- CS488 -- Introduction to Computer Graphics
--
-- puppet.lua
--

-- materials
red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

---- rootnode ----
rootnode = gr.node('rootnode')

-- torso --
torso = gr.sphere('torso');
rootnode:add_child(torso);
torso:set_material(white)

  ---- shoulders ----
  shoulders = gr.sphere('shoulders')
  shoulders:scale(1.5, 0.2, 1)
  shoulders:translate(0, 0.5, 0);
  torso:add_child(shoulders)

    ---- left arm ----

      -- left upper arm --
      left_upper_arm = gr.sphere('left_upper_arm');
      shoulders:add_child(left_upper_arm);

      -- left forearm --
      left_forearm = gr.sphere('left_forearm');
      left_upper_arm:add_child(left_forearm);

      -- left hand --
      left_hand = gr.sphere('left_hand');
      left_forearm:add_child(left_hand);

    ---- right arm ----

      -- right upper arm --
      right_upper_arm = gr.sphere('right_upper_arm');
      shoulders:add_child(right_upper_arm);

      -- right forearm --
      right_forearm = gr.sphere('right_forearm');
      right_upper_arm:add_child(right_forearm);

      -- right hand --
      right_hand = gr.sphere('right_hand');
      right_forearm:add_child(right_hand);

    ---- head and neck ----

      -- neck --
      neck = gr.sphere('neck');
      shoulders:add_child(neck);

      -- head --
      head = gr.sphere('head');
      neck:add_child(head);

  ---- hips ----
  hips = gr.sphere('hips')
  torso:add_child(hips)
  --hips:scale(0.1, 2.0, 0.1)
  --hips:set_material(red)

    ---- left leg ----

      -- left thigh --
      left_thigh = gr.sphere('left_thigh');
      hips:add_child(left_thigh);

      -- left calf --
      left_calf = gr.sphere('left_calf');
      left_thigh:add_child(left_calf);

      -- left foot --
      left_foot = gr.sphere('left_foot');
      left_calf:add_child(left_foot);

    ---- right leg ----

      -- right thigh --
      right_thigh = gr.sphere('right_thigh');
      hips:add_child(right_thigh);

      -- right calf --
      right_calf = gr.sphere('right_calf');
      right_thigh:add_child(right_calf);

      -- right foot --
      right_foot = gr.sphere('right_foot');
      right_calf:add_child(right_foot);

--[[
s2 = gr.sphere('s2')
rootnode:add_child(s2)
s2:translate(2.0, -2.0, 0.0)
s2:rotate('z', -90.0)
s2:scale(0.1, 2.0, 0.1)
s2:set_material(blue)

s3 = gr.sphere('s3')
rootnode:add_child(s3)
s3:scale(0.1, 0.1, 2.0)
s3:translate(0.0, -20.0, 1.0)
s3:set_material(green)
]]

rootnode:translate(-0.75, 0.25, -10.0)
rootnode:rotate('y', -20.0)

return rootnode
