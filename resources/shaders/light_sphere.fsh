#version 330 core
out vec4 FragColor;

struct Light {
  vec3 pos;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Camera {
  vec3 pos;
};

uniform vec3 lightColor;
uniform Camera camera;
uniform Light light;

in vec3 vPos;
in vec3 vNormal;

void main() {
  vec3 ambient = light.ambient;
  vec3 lightDir = normalize(light.pos - vPos);
  float diff = max(dot(vNormal, lightDir), 0.0f);
  vec3 diffuse = light.diffuse * diff;

  vec3 viewDir = normalize(camera.pos - vPos);
  vec3 reflectionDirection = reflect(-lightDir, vNormal);
  float specAmount = pow(max(dot(viewDir, reflectionDirection), 0.0f), 8);
  vec3 specular = light.specular * specAmount;
  
  FragColor = vec4(lightColor, 1.0f) * vec4(ambient + diffuse + specular, 1.0f);
}