#version 150
in vec2 pos;
in vec2 uv;
in vec4 color;

uniform mat4 projection;

out vec2 Frag_UV;
out vec4 Frag_Color;

void main() {
  Frag_UV = uv;
  Frag_Color = color;
  gl_Position = projection * vec4(pos.xy, 0.0f, 1.0);
}