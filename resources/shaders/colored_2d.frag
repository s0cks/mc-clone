#version 150
out vec4 Frag_Color;

in vec2 vUv;
in vec4 vColor;

void main() {
  Frag_Color = vColor;
}