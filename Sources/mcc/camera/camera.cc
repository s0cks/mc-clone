#include "mcc/camera/camera.h"
#include "mcc/thread_local.h"

namespace mcc {
  void PerspectiveCamera::Update() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
  }

  static ThreadLocal<OrthoCamera> ortho_;

  OrthoCamera* OrthoCamera::Initialize(const glm::vec2 size) {
    const auto camera = new OrthoCamera(size);
    Set(camera);
    return camera;
  }

  OrthoCamera* OrthoCamera::Get() {
    return ortho_.Get();
  }

  void OrthoCamera::Set(OrthoCamera* value) {
    ortho_.Set(value);
  }
}