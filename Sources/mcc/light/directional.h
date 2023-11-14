#ifndef MCC_DIRECTIONAL_LIGHT_H
#define MCC_DIRECTIONAL_LIGHT_H

#include "mcc/gfx.h"
#include "mcc/light/light.h"

namespace mcc::light {
  struct DirectionalLight : public Light {
    glm::vec3 direction;
  };
}

#endif //MCC_DIRECTIONAL_LIGHT_H