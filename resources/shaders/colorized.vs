#version 330 core
layout (location = 0) in vec3 pos;

uniform vec3 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 sColor;

void main() {
  gl_Position = projection * view * model * vec4(pos, 1.0);
  sColor = color;
}