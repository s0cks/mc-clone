#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec4 color;

uniform mat4 projection;

out vec4 vColor;

void main() {
  vColor = color;
  gl_Position = projection * vec4(pos.xy, 0.0f, 1.0);
}