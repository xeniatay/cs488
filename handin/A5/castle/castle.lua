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

--[[

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
      left_upper_arm_node:translate(-1.3, -0.7, 0);
      left_upper_joint:add_child(left_upper_arm_node);

      left_upper_arm_primt = gr.cube('left_upper_arm_primt');
      left_upper_arm_primt:scale( 0.15, 0.7, 1 );
      left_upper_arm_primt:set_material(blue);
      left_upper_arm_node:add_child(left_upper_arm_primt);

      -- left forearm joint --
      left_forearm_joint = gr.joint(
        'left_forearm_joint',
        {0, 0, 0},
        {0, 0, 160}
      );
      left_upper_arm_node:add_child(left_forearm_joint);

      -- left forearm --
      left_forearm_node = gr.node('left_forearm_node');
      left_forearm_node:translate( 0, -0.7, 0 );
      left_forearm_joint:add_child(left_forearm_node);

      left_forearm_primt = gr.cube('left_forearm_primt');
      left_forearm_primt:scale( 0.15, 0.7, 1 );
      left_forearm_primt:set_material(green);
      left_forearm_node:add_child(left_forearm_primt);

      -- left hand joint --
      left_hand_joint = gr.joint(
        'left_hand_joint',
        {0, 0, 0},
        {0, 0, 70}
      );
      left_forearm_node:add_child(left_hand_joint);

      -- left hand --
      left_hand_node = gr.node('left_hand_node');
      left_hand_node:translate( -0.2, -0.7, 0 );
      left_hand_joint:add_child(left_hand_node);

      left_hand_primt = gr.cube('left_hand_primt');
      left_hand_primt:scale( 0.1, 0.2, 1 );
      left_hand_primt:set_material(black);
      left_hand_node:add_child(left_hand_primt);


    ---- right arm ----

      -- right upper joint --
      right_upper_joint = gr.joint(
        'right_upper_joint',
        {0, 0, 0},
        {0, 0, 200}
      );
      shoulder_node:add_child(right_upper_joint);

      -- right upper arm --
      right_upper_arm_node = gr.node('right_upper_arm_node');
      right_upper_arm_node:translate(1.3, -0.7, 0);
      right_upper_joint:add_child(right_upper_arm_node);

      right_upper_arm_primt = gr.cube('right_upper_arm_primt');
      right_upper_arm_primt:scale( 0.15, 0.7, 1 );
      right_upper_arm_primt:set_material(blue);
      right_upper_arm_node:add_child(right_upper_arm_primt);

      -- right forearm joint --
      right_forearm_joint = gr.joint(
        'right_forearm_joint',
        {0, 0, 0},
        {0, 0, 160}
      );
      right_upper_arm_node:add_child(right_forearm_joint);

      -- right forearm --
      right_forearm_node = gr.node('right_forearm_node');
      right_forearm_node:translate( 0, -0.7, 0 );
      right_forearm_joint:add_child(right_forearm_node);

      right_forearm_primt = gr.cube('right_forearm_primt');
      right_forearm_primt:scale( 0.15, 0.7, 1 );
      right_forearm_primt:set_material(green);
      right_forearm_node:add_child(right_forearm_primt);

      -- right hand joint --
      right_hand_joint = gr.joint(
        'right_hand_joint',
        {0, 0, 0},
        {0, 0, 70}
      );
      right_forearm_node:add_child(right_hand_joint);

      -- right hand --
      right_hand_node = gr.node('right_hand_node');
      right_hand_node:translate( 0.2, -0.7, 0 );
      right_hand_joint:add_child(right_hand_node);

      right_hand_primt = gr.cube('right_hand_primt');
      right_hand_primt:scale( 0.1, 0.2, 1 );
      right_hand_primt:set_material(black);
      right_hand_node:add_child(right_hand_primt);


    ---- head and neck ----

      -- neck --
      neck_node = gr.node('neck_node');
      neck_node:translate(0, 0.5, 0);
      shoulder_node:add_child(neck_node);

      neck_primt = gr.cube('neck_primt');
      neck_primt:set_material(blue);
      neck_primt:scale(0.3, 0.2, 1);
      neck_node:add_child(neck_primt);

      -- head joint --
      head_joint = gr.joint(
        'head_joint',
        {0, 0, 0},
        {0, 0, 30}
      );
      neck_node:add_child(head_joint);

      -- head --
      head_node = gr.node('head_node');
      head_node:translate( 0, 1.0, 0 );
      head_joint:add_child(head_node);

      head_primt = gr.cube('head_primt');
      head_primt:scale( 0.7, 0.7, 1 );
      head_primt:set_material(black);
      head_node:add_child(head_primt);

      -- nose --
      nose_primt = gr.cube('nose_primt');
      nose_primt:translate(0, 0, 1.0);
      nose_primt:scale( 0.08, 0.0005, 1 );
      nose_primt:set_material(green);
      head_node:add_child(nose_primt);

  ---- hips_primt ----
  hips_node = gr.node('hips_node');
  hips_node:translate(0, -2.0, 0);
  mh_f_node:add_child(hips_node);

  hips_primt = gr.cube('hips_primt');
  hips_primt:scale(1.8, 0.3, 1);
  hips_primt:set_material(red);
  hips_node:add_child(hips_primt);

    ---- left leg ----

      -- left upper joint --
      left_upper_joint = gr.joint(
        'left_upper_joint',
        {0, 0, 0},
        {0, 0, 200}
      );
      hips_node:add_child(left_upper_joint);

      -- left upper leg --
      left_thigh_node = gr.node('left_thigh_node');
      left_thigh_node:translate(-1.3, -1.0, 0);
      left_upper_joint:add_child(left_thigh_node);

      left_thigh_primt = gr.cube('left_thigh_primt');
      left_thigh_primt:scale( 0.15, 1.0, 1 );
      left_thigh_primt:set_material(blue);
      left_thigh_node:add_child(left_thigh_primt);

      -- left calf joint --
      left_calf_joint = gr.joint(
        'left_calf_joint',
        {0, 0, 0},
        {0, 0, 160}
      );
      left_thigh_node:add_child(left_calf_joint);

      -- left calf --
      left_calf_node = gr.node('left_calf_node');
      left_calf_node:translate( 0, -1.0, 0 );
      left_calf_joint:add_child(left_calf_node);

      left_calf_primt = gr.cube('left_calf_primt');
      left_calf_primt:scale( 0.15, 1.0, 1 );
      left_calf_primt:set_material(green);
      left_calf_node:add_child(left_calf_primt);

      -- left foot joint --
      left_foot_joint = gr.joint(
        'left_foot_joint',
        {0, 0, 0},
        {0, 0, 70}
      );
      left_calf_node:add_child(left_foot_joint);

      -- left foot --
      left_foot_node = gr.node('left_foot_node');
      left_foot_node:translate( -0.2, -1.0, 0 );
      left_foot_joint:add_child(left_foot_node);

      left_foot_primt = gr.cube('left_foot_primt');
      left_foot_primt:scale( 0.1, 0.2, 1 );
      left_foot_primt:set_material(black);
      left_foot_node:add_child(left_foot_primt);

    ---- right leg ----

      -- right upper joint --
      right_upper_joint = gr.joint(
        'right_upper_joint',
        {0, 0, 0},
        {0, 0, 200}
      );
      hips_node:add_child(right_upper_joint);

      -- right upper leg --
      right_thigh_node = gr.node('right_thigh_node');
      right_thigh_node:translate(1.3, -1.0, 0);
      right_upper_joint:add_child(right_thigh_node);

      right_thigh_primt = gr.cube('right_thigh_primt');
      right_thigh_primt:scale( 0.15, 1.0, 1 );
      right_thigh_primt:set_material(blue);
      right_thigh_node:add_child(right_thigh_primt);

      -- right calf joint --
      right_calf_joint = gr.joint(
        'right_calf_joint',
        {0, 0, 0},
        {0, 0, 160}
      );
      right_thigh_node:add_child(right_calf_joint);

      -- right calf --
      right_calf_node = gr.node('right_calf_node');
      right_calf_node:translate( 0, -1.0, 0 );
      right_calf_joint:add_child(right_calf_node);

      right_calf_primt = gr.cube('right_calf_primt');
      right_calf_primt:scale( 0.15, 1.0, 1 );
      right_calf_primt:set_material(green);
      right_calf_node:add_child(right_calf_primt);

      -- right foot joint --
      right_foot_joint = gr.joint(
        'right_foot_joint',
        {0, 0, 0},
        {0, 0, 70}
      );
      right_calf_node:add_child(right_foot_joint);

      -- right foot --
      right_foot_node = gr.node('right_foot_node');
      right_foot_node:translate( 0.2, -1.0, 0 );
      right_foot_joint:add_child(right_foot_node);

      right_foot_primt = gr.cube('right_foot_primt');
      right_foot_primt:scale( 0.1, 0.2, 1 );
      right_foot_primt:set_material(black);
      right_foot_node:add_child(right_foot_primt);
]]

rootnode:translate(0, 0, 5.0)
--rootnode:rotate('y', -20.0)

return rootnode
