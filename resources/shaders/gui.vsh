#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 inColor;

void main() {
  gl_Position = projection * view * model * vec4(pos, 1.0f, 1.0);
  inColor = color;
}