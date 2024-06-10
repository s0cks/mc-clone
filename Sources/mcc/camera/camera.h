#ifndef MCC_CAMERA_H
#define MCC_CAMERA_H

#include "mcc/glm.h"
#include "mcc/object.h"

namespace gpu {
  struct CameraData {
    glm::mat4 projection;
    glm::mat4 view;   
  };
}

namespace mcc::camera {
  class Camera : public Object {
  protected:
    Camera() = default;
    explicit Camera(const Metadata& meta):
      Object(meta) {
    }
  public:
    virtual ~Camera() = default;
    virtual const glm::mat4& GetProjection() const = 0;
    virtual const glm::mat4& GetView() const = 0;
  };
}

#endif //MCC_CAMERA_H