#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 in_Tex;
out vec3 in_Color;
out vec3 in_Pos;
out vec3 in_Normal;

void main() {
  in_Pos = vec3(model * vec4(pos, 1.0f));
  gl_Position = projection * view * vec4(in_Pos, 1.0f);
  in_Tex = uv;
  in_Color = color;
  in_Normal = normal;
}