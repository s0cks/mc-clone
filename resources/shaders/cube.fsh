#version 330 core

uniform sampler2D tex;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 camPos;

in vec3 in_Color;
in vec2 in_Tex;
in vec3 in_Pos;

out vec4 FragColor;

void main() {
  FragColor = texture(tex, in_Tex) * vec4(lightColor, 1.0f);
}