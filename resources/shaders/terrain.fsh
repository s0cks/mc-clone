#version 330 core
out vec4 FragColor;

layout (std140) uniform Camera {
  vec4 pos;
  mat4 projection;
  mat4 view;
} camera;

layout (std140) uniform DirectionalLight {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 direction;
} dir_light;

layout (std140) uniform PointLight {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 position;
  float constant;
  float linear;
  float quadratic;
  float pad;
} point_light;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform sampler2D tex;
uniform Material material;

in vec3 vPos;
in vec3 vNormal;
in vec2 vUv;
in vec3 vColor;

vec3 calculateDirectionalLight(vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-dir_light.direction.xyz);
  float diff = max(dot(normal, lightDir), 0.0f);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 ambient = dir_light.ambient.xyz * vec3(texture(tex, vUv));
  vec3 diffuse = dir_light.diffuse.xyz * diff * vec3(texture(tex, vUv));
  vec3 specular = dir_light.specular.xyz * spec * vec3(texture(tex, vUv));
  return (ambient + diffuse + specular);
}

vec3 calculatePointLight(vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(point_light.position.xyz - fragPos);
  vec3 halfDir = normalize(lightDir + viewDir);
  float diff = max(dot(normal, lightDir), 0.0f);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(normal, halfDir), 0.0f), material.shininess);
  float distance = length(point_light.position.xyz - fragPos);
  float atten = 1.0f / (point_light.constant + point_light.linear + distance + point_light.quadratic * (distance * distance));
  vec3 ambient = (dir_light.ambient.xyz * vec3(texture(tex, vUv))) * atten;
  vec3 diffuse = (dir_light.diffuse.xyz * diff * vec3(texture(tex, vUv))) * atten;
  vec3 specular = (dir_light.specular.xyz * spec * vec3(texture(tex, vUv))) * atten;
  return (ambient + diffuse + specular);
}

void main() {
  vec3 normal = normalize(vNormal);
  vec3 viewDir = normalize(camera.pos.xyz - vPos);

  vec3 result = calculateDirectionalLight(normal, viewDir);
  result += calculatePointLight(normal, vPos, viewDir);
  FragColor = vec4(result, 1.0f);
}