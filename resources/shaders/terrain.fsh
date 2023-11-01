#version 330 core

uniform sampler2D tex0;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;

uniform vec3 globalLightPos;
uniform vec3 globalLightColor;
uniform float globalLightIntensity;

in vec3 inColor;
in vec2 inTex;
in vec3 in_Pos;

out vec4 FragColor;

void main() {
  float ambient = lightIntensity;
  vec3 normal = vec3(0.0f, 1.0f, 0.0f);
  vec3 lightDirection = normalize(lightPos - in_Pos);
  float diffuse = max(dot(normal, lightDirection), 0.0f);

  float specLight = 0.9f;
  vec3 viewDirection = normalize(cameraPos - in_Pos);
  vec3 reflectionDirection = reflect(-lightDirection, normal);
  float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
  float specular = specAmount * specLight;

  FragColor = texture(tex0, inTex) * vec4(lightColor, 1.0f) * (diffuse * ambient * specular);
}