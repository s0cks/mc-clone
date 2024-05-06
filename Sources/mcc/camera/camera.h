#ifndef MCC_CAMERA_H
#define MCC_CAMERA_H

#include "mcc/glm.h"

namespace gpu {
  struct CameraData {
    glm::mat4 projection;
    glm::mat4 view;   
  };
}

namespace mcc::camera {
  class Camera {
  protected:
    Camera() = default;
  public:
    virtual ~Camera() = default;
    virtual std::string ToString() const = 0;
    virtual const glm::mat4& GetProjection() const = 0;
    virtual const glm::mat4& GetView() const = 0;
  };
}

#endif //MCC_CAMERA_H