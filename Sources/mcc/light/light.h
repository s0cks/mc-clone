#ifndef MCC_LIGHT_H
#define MCC_LIGHT_H

namespace mcc::light {
  struct Light {
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
  };
}

#endif //MCC_LIGHT_H