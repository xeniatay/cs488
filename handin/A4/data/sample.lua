-- A scene full of sphere strings

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)

scene_root = gr.node('root')

---- SPHERE STRING

r = 40;
numSpheres = 20;
colour_r = 0.2;
colour_g = 0.1;
colour_b = 0.8;

for i = 1, numSpheres do

  -- sphere calculations
  theta = (math.pi / numSpheres) * i;
  y = -300 + (math.tan(theta) * r ) * i;
  x = -100 + ((r / math.cos(theta)) ) * i;
  z = (i * r) + i;

  -- generate spheres
  sphereStr = "c" .. i;
  sphere = gr.nh_sphere(sphereStr, {x, -y, z}, r)
  scene_root:add_child(sphere)

  -- material
  colour_r = (colour_r + (i / 10)) % 1;
  colour_g = (colour_g + (i / 4)) % 1;
  colour_b = (colour_b + (i / 7)) % 1;
  mat = gr.material({colour_r, colour_g, colour_b}, {0.5, 0.7, 0.5}, 25)
  sphere:set_material(mat)
end

---- END SPHERE STRING

mat1 = gr.material({0.7, 1.0, 0.2}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)

b1 = gr.nh_box('b1', {-200, -125, 0}, 100)
scene_root:add_child(b1)
b1:set_material(mat1)

-- A small stellated dodecahedron.

require('smstdodeca')

steldodec:set_material(mat3)
scene_root:add_child(steldodec)

white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'sample.png', 500, 500,
    {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {white_light, orange_light})

