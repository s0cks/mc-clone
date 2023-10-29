#version 330 core

uniform sampler2D tex0;

in vec3 inColor;
in vec2 inTex;

out vec4 FragColor;

void main() {
  FragColor = texture(tex0, inTex);
}