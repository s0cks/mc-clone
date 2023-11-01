#ifndef MCC_AMBIENT_LIGHT_H
#define MCC_AMBIENT_LIGHT_H

#include "mcc/gfx.h"

namespace mcc {
  struct AmbientLight {
    glm::vec3 color;
    float intensity;
  };
}

#endif //MCC_AMBIENT_LIGHT_H