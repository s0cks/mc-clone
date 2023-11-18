#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

layout (std140) uniform Camera {
  vec4 pos;
  mat4 projection;
  mat4 view;
} camera;

struct Light {
  vec3 pos;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform vec3 lightColor;
uniform Light light;

in vec3 vPos;
in vec3 vNormal;

void main() {
  vec3 normal = normalize(vNormal);
  vec3 ambient = light.ambient;
  vec3 lighting = vec3(0.0f);

  // diffuse
  vec3 lightDir = normalize(light.pos - vPos);
  float diff = max(dot(lightDir, vNormal), 0.0);
  vec3 atten = lightColor * diff * (light.diffuse * diff);      
  // attenuation (use quadratic as we have gamma correction)
  float distance = length(vPos - light.pos);
  atten *= 1.0 / (distance * distance);
  lighting += atten;

  vec3 result = ambient + lighting;
  // check whether result is higher than some threshold, if so, output as bloom threshold color
  float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
  if(brightness > 1.0) {
    BrightColor = vec4(result, 1.0);
  } else {
    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
  FragColor = vec4(result, 1.0);
}