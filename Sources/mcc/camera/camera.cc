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

  static ThreadLocal<PerspectiveCamera> perspective_;

  PerspectiveCamera* PerspectiveCamera::Initialize(const glm::vec2 size) {
    const auto camera = new PerspectiveCamera(size, glm::vec3(0.0f), glm::vec3(0.0f));
    Set(camera);
    return camera;
  }

  PerspectiveCamera* PerspectiveCamera::Get() {
    return perspective_.Get();
  }

  void PerspectiveCamera::Set(PerspectiveCamera* camera) {
    perspective_.Set(camera);
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