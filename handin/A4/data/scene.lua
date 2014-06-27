-- A scene full of sphere strings

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)

scene_root = gr.node('root')

---- SPHERE STRING

r = 10;
numSpheres = 30;
colour_r = 0.1;
colour_g = 0.3;
colour_b = 0.6;

for i = 1, numSpheres do

  -- sphere calculations
  theta = (math.pi / numSpheres) * i;
  y = -200 + ( ( ( math.tan(theta) * r ) * i) % 400);
  x = -200 + ( ( ( (r / math.cos(theta)) ) * i) % 400);
  z = (i * r) + i;

  -- generate spheres
  sphereStr = "c" .. i;
  sphere = gr.nh_sphere(sphereStr, {x, y, z}, r)
  scene_root:add_child(sphere)

  -- material
  colour_r = (colour_r + (i / 10)) % 1;
  colour_g = (colour_g + (i / 4)) % 1;
  colour_b = (colour_b + (i / 7)) % 1;
  mat = gr.material({colour_r, colour_g, colour_b}, {0.5, 0.7, 0.5}, 25)
  sphere:set_material(mat)
end

---- END SPHERE STRING

---- SPHERE STRING

r = 40;
numSpheres = 30;
colour_r = 0.2;
colour_g = 0.1;
colour_b = 0.8;

for i = 1, numSpheres do

  -- sphere calculations
  theta = (math.pi / numSpheres) * i;
  y = -150 + (math.tan(theta) * r ) * i;
  x = -250 + ((r / math.cos(theta)) ) * i;
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


---- SPHERE STRING

r = 20;
numSpheres = 30;
colour_r = 0.9;
colour_g = 0.2;
colour_b = 0.8;

for i = 1, numSpheres do

  -- sphere calculations
  theta = (math.pi / numSpheres) * i;
  y = -200 + (math.tan(theta) * r ) * i;
  x = -200 + ((r / math.cos(theta)) ) * i;
  z = (i * r) + i;

  -- generate spheres
  sphereStr = "c" .. i;
  mult = 1;
  if (i % 2 == 0) then
    mult = -1;
  end
  sphere = gr.nh_sphere(sphereStr, {mult * x, -1 * mult * y, z}, r)
  scene_root:add_child(sphere)

  -- material
  colour_r = (colour_r + (i / 10)) % 1;
  colour_g = (colour_g + (i / 4)) % 1;
  colour_b = (colour_b + (i / 7)) % 1;
  mat = gr.material({colour_r, colour_g, colour_b}, {0.5, 0.7, 0.5}, 25)
  sphere:set_material(mat)
end

---- END SPHERE STRING

---- SPHERE STRING

r = 50;
numSpheres = 200;
colour_r = 0.1;
colour_g = 0.2;
colour_b = 0.3;

for i = 1, numSpheres do

  -- sphere calculations
  theta = (math.pi / numSpheres) * i;
  y = -200 + (math.tan(theta) * r ) * i;
  x = -200 + ((r / math.cos(theta)) ) * i;
  z = (i * r) + i;

  -- generate spheres
  sphereStr = "c" .. i;
  mult = 1;
  if (i % 2 == 0) then
    mult = -1;
  end
  sphere = gr.nh_sphere(sphereStr, {mult * x, -1 * mult * y, z}, r)
  scene_root:add_child(sphere)

  -- material
  colour_r = (colour_r + (i / 10)) % 1;
  colour_g = (colour_g + (i / 4)) % 1;
  colour_b = (colour_b + (i / 7)) % 1;
  mat = gr.material({colour_r, colour_g, colour_b}, {0.5, 0.7, 0.5}, 25)
  sphere:set_material(mat)
end

---- END SPHERE STRING

---- SPHERE STRING

r = 10;
numSpheres = 30;
colour_r = 0.6;
colour_g = 0.7;
colour_b = 0.1;

for i = 1, numSpheres do

  -- sphere calculations
  r = r + i;
  theta = (math.pi / numSpheres) * i;
  y = -200 + (math.tan(theta) * r ) * i;
  x = -200 + ((r / math.cos(theta)) ) * i;
  z = (i * i);

  -- generate spheres
  sphereStr = "c" .. i;
  sphere = gr.nh_sphere(sphereStr, {-x, -y, z}, r)
  scene_root:add_child(sphere)

  -- material
  colour_r = (colour_r + (i / 10)) % 1;
  colour_g = (colour_g + (i / 4)) % 1;
  colour_b = (colour_b + (i / 7)) % 1;
  mat = gr.material({colour_r, colour_g, colour_b}, {0.5, 0.7, 0.5}, 25)
  sphere:set_material(mat)
end

---- END SPHERE STRING

---- SPHERE STRING

r = 10;
numSpheres = 20;
colour_r = 0.2;
colour_g = 0.9;
colour_b = 0.4;

for i = 1, numSpheres do

  -- sphere calculations
  r = r + 2;
  theta = (math.pi / numSpheres) * i;
  y = -200 + ( ( ( math.tan(theta) * r ) * i) % 1000);
  x = -200 + ( ( ( (r / math.cos(theta)) ) * i) % 1000);
  z = i;

  -- generate spheres
  sphereStr = "c" .. i;
  mult = 1;
  if (i % 2 == 0) then
    mult = -1;
  end
  sphere = gr.nh_sphere(sphereStr, {mult * x, -1 * mult * y, z}, r)
  scene_root:add_child(sphere)

  -- material
  colour_r = (colour_r + (i / 10)) % 1;
  colour_g = (colour_g + (i / 4)) % 1;
  colour_b = (colour_b + (i / 7)) % 1;
  mat = gr.material({colour_r, colour_g, colour_b}, {0.5, 0.7, 0.5}, 25)
  sphere:set_material(mat)
end

---- END SPHERE STRING

white_light = gr.light({-600.0, 450.0, 30.0}, {0.9, 0.9, 0.9}, {1, 0, 0})

gr.render(scene_root, 'scene.png', 500, 500,
    {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {white_light})

