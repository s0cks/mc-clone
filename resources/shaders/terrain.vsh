#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 inTex;
out vec3 inColor;
out vec3 in_Pos;

void main() {
  in_Pos = vec3(model * vec4(pos, 1.0f));
  gl_Position = projection * view * model * vec4(in_Pos, 1.0);
  inColor = color;
  inTex = uv;
}