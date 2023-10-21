#version 330 core
in vec2 texel;
out vec4 FragColor;

uniform sampler2D texture1;

void main() {
  FragColor = texture(texture1, texel);
}