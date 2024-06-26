#version 330 core
out vec4 FragColor;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 pos;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 FragPos;
in vec3 FragNormal;

void main() {
  vec3 ambient = light.ambient * material.ambient;
  vec3 norm = normalize(FragNormal);
  vec3 lightDir = normalize(light.pos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = light.diffuse * (diff * material.diffuse);
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 specular = light.specular * (spec * material.specular);
  
  FragColor = vec4(ambient + diffuse + specular, 1.0f);
}