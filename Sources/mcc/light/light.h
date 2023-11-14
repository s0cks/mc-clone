#ifndef MCC_LIGHT_H
#define MCC_LIGHT_H

namespace mcc::light {
  struct Light {
  public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
  };
}

#endif //MCC_LIGHT_H