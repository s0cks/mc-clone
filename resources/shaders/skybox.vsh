#version 330 core
layout (location = 0) in vec3 pos;

layout (std140) uniform Camera {
  vec4 pos;
  mat4 projection;
  mat4 view;
} camera;

uniform mat4 model;
out vec3 uv;

void main() {
  uv = pos;
  gl_Position = camera.projection * camera.view * vec4(pos, 1.0f);
}