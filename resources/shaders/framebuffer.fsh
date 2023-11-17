#version 330 core
out vec4 Frag_Color;

in vec2 vUv;

uniform sampler2D tex;

void main() {
  Frag_Color = texture(tex, vUv);
}