#ifndef MCC_CAMERA_H
#define MCC_CAMERA_H

#include "mcc/glm.h"

namespace mcc::camera {
  class Camera {
  protected:
    Camera() = default;
  public:
    virtual ~Camera() = default;
  };
}

#endif //MCC_CAMERA_H