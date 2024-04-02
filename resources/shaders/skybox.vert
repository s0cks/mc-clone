#version 330 core
layout (location = 0) in vec3 pos;

layout (std140) uniform Camera {
  vec4 pos;
  mat4 projection;
  mat4 view;
} camera;

uniform mat4 model;
out vec3 vUv;

void main() {
  vUv = pos;
  mat4 view = mat4(mat3(camera.view));
  gl_Position = camera.projection * view * vec4(pos, 1.0f);
}