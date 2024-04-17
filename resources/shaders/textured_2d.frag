#version 150
out vec4 Frag_Color;

uniform sampler2D tex;
in vec2 vUv;
in vec4 vColor;
in vec4 oColor;

void main() {
  Frag_Color = texture(tex, vUv);
}