#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
out vec3 vPos;

void main() {
  vPos = pos;
  gl_Position = projection * mat4(mat3(view)) * vec4(pos, 1.0f);
}