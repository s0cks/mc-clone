#version 150
out vec4 Frag_Color;

in vec2 vUv;
in vec4 vColor;
in vec4 oColor;

void main() {
  Frag_Color = oColor;
}