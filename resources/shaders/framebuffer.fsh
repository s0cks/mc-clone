#version 330 core
out vec4 Frag_Color;

in vec2 vUv;

uniform sampler2D tex;
uniform bool hdr;
uniform float gamma;
uniform float exposure;

void main() {
  vec3 color = texture(tex, vUv).rgb;
  vec3 result;
  if(hdr) {
    result = vec3(1.0f) - exp(-color * exposure);
    result = pow(result, vec3(1.0f / gamma));
  } else {
    result = pow(color, vec3(1.0f / gamma));
  }
  Frag_Color = vec4(result, 1.0f);
}