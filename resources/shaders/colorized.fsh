#version 330 core
in vec3 sColor;
out vec4 FragColor;

void main() {
  FragColor = vec4(sColor, 1.0);
}