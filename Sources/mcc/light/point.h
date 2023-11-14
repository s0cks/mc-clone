#ifndef MCC_POINT_LIGHT_H
#define MCC_POINT_LIGHT_H

#include "mcc/gfx.h"
#include "mcc/light/light.h"

namespace mcc::light {
  struct PointLight : public Light {
    glm::vec3 direction;
  };
}


#endif //MCC_POINT_LIGHT_H