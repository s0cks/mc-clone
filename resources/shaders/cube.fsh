#version 330 core

uniform sampler2D tex;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 camPos;

in vec3 in_Color;
in vec2 in_Tex;
in vec3 in_Pos;
in vec3 in_Normal;

out vec4 FragColor;

void main() {
  float ambient = lightIntensity;
  vec3 normal = normalize(in_Normal);
  vec3 lightDirection = normalize(lightPos - in_Pos);
  float diffuse = max(dot(normal, lightDirection), 0.0f);

  FragColor = texture(tex, in_Tex) * vec4(lightColor, 1.0f) * (diffuse * ambient);
}