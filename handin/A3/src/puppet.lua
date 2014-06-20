--
-- CS488 -- Introduction to Computer Graphics
--
-- puppet.lua
--

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

-- materials
red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10);
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10);
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10);
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10);
black = gr.material({0.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10);

---- rootnode ----
rootnode = gr.node('rootnode');

-- torso --
torso = gr.sphere('torso');
torso:set_material(black);
rootnode:add_child(torso);

  ---- shoulder_prmt ----
  shoulder_node = gr.node('shoulder_node');
  shoulder_node:translate(0, 1.0, 0);
  torso:add_child(shoulder_node);

  shoulder_prmt = gr.sphere('shoulder_prmt');
  shoulder_prmt:scale(1.5, 0.2, 1);
  shoulder_prmt:set_material(red);
  shoulder_node:add_child(shoulder_prmt);

    ---- left arm ----

      -- left upper joint --
      left_upper_joint = gr.joint(
        'left_upper_joint',
        {0, 0, 0},
        {0, 0, 200}
      );
      shoulder_node:add_child(left_upper_joint);

      -- left upper arm --
      left_upper_arm_node = gr.node('left_upper_arm_node');
      left_upper_arm_node:translate(-1.3, -1.0, 0);
      left_upper_joint:add_child(left_upper_arm_node);

      left_upper_arm_prmt = gr.sphere('left_upper_arm_prmt');
      left_upper_arm_prmt:scale( 0.15, 1.0, 1 );
      left_upper_arm_prmt:set_material(blue);
      left_upper_arm_node:add_child(left_upper_arm_prmt);

      -- left forearm joint --
      left_forearm_joint = gr.joint(
        'left_forearm_joint',
        {0, 0, 0},
        {0, 0, 160}
      );
      left_upper_arm_node:add_child(left_forearm_joint);

      -- left forearm --
      left_forearm_node = gr.node('left_forearm_node');
      left_forearm_node:translate( 0, -1.0, 0 );
      left_forearm_joint:add_child(left_forearm_node);

      left_forearm_prmt = gr.sphere('left_forearm_prmt');
      left_forearm_prmt:scale( 0.15, 1.0, 1 );
      left_forearm_prmt:set_material(green);
      left_forearm_node:add_child(left_forearm_prmt);

      -- left hand joint --
      left_hand_joint = gr.joint(
        'left_hand_joint',
        {0, 0, 0},
        {0, 0, 70}
      );
      left_forearm_node:add_child(left_hand_joint);

      -- left hand --
      left_hand_node = gr.node('left_hand_node');
      left_hand_node:translate( -0.2, -1.0, 0 );
      left_hand_joint:add_child(left_hand_node);

      left_hand_prmt = gr.sphere('left_hand_prmt');
      left_hand_prmt:scale( 0.1, 0.2, 1 );
      left_hand_prmt:set_material(black);
      left_hand_node:add_child(left_hand_prmt);

--[[
    ---- right arm ----

      -- right upper arm --
      right_upper_arm = gr.sphere('right_upper_arm');
      shoulder_prmt:add_child(right_upper_arm);

      -- right forearm --
      right_forearm = gr.sphere('right_forearm');
      right_upper_arm:add_child(right_forearm);

      -- right hand --
      right_hand = gr.sphere('right_hand');
      right_forearm:add_child(right_hand);

    ---- head and neck ----

      -- neck --
      neck = gr.sphere('neck');
      shoulder_prmt:add_child(neck);

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
]]
rootnode:translate(-0.75, 0.25, -10.0)
--rootnode:rotate('y', -20.0)

return rootnode
