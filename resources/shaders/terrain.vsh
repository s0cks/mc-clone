#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vUv;
out vec3 vColor;
out vec3 vPos;
out vec3 vNormal;

void main() {
  vPos = pos;
  vNormal = vec3(0.0f, 1.0f, 0.0f);
  vColor = color;
  vUv = uv;
  gl_Position = projection * view * model * vec4(pos, 1.0f);
}