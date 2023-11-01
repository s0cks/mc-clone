#version 330 core

uniform sampler2D tex;

in vec2 inUv;

out vec4 FragColor;

void main() {
  FragColor = texture(tex, inUv) * vec4(1.0);
}