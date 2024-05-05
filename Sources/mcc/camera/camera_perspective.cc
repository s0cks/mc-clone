#include "mcc/camera/camera_perspective.h"

#include <sstream>
#include "mcc/thread_local.h"
#include "mcc/window/window.h"

namespace mcc::camera {
  PerspectiveCameraData::PerspectiveCameraData():
    viewport(0.0f),
    up(glm::vec3(0.0f, 1.0f, 0.0f)),
    pos(0.0f),
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    right(0.0f),
    projection(1.0f),
    view(1.0f),
    yaw(-90.0f),
    pitch(0.0f),
    speed(0.5f),
    sensitivity(0.1f),
    zoom(45.0f) {
    projection = CalculateProjection();
    view = CalculateView();
  }

  PerspectiveCameraData::PerspectiveCameraData(const glm::vec2& viewport_size,
                                               const glm::vec3& f,
                                               const glm::vec3& p,
                                               const glm::vec3& u):
    viewport(0, 0, viewport_size),
    up(u),
    pos(p),
    front(f),
    right(glm::vec3(0.0f)),
    projection(glm::mat4(1.0f)),
    view(glm::mat4(1.0f)),
    yaw(-90.0f),
    pitch(0.0f),
    speed(0.5f),
    sensitivity(0.1f),
    zoom(45.0f) {
    projection = CalculateProjection();
    view = CalculateView();
  }

  glm::mat4 PerspectiveCameraData::CalculateProjection() const {
    const auto v = viewport[2] / viewport[3];
    return glm::perspective(glm::radians(zoom), v, 0.1f, 100.0f);
  }

  glm::mat4 PerspectiveCameraData::CalculateView() const {
    return glm::lookAt(pos, pos + front, up);
  }

  PerspectiveCamera::PerspectiveCamera(const PerspectiveCameraData& data):
    Camera(),
    data_(data) {
  }

  PerspectiveCamera::~PerspectiveCamera() {
    if(on_mouse_moved_.is_subscribed())
      on_mouse_moved_.unsubscribe();
    if(on_key_pressed_.is_subscribed())
      on_key_pressed_.unsubscribe();
  }

  void PerspectiveCamera::Update() {
    glm::vec3 front;
    front.x = cos(glm::radians(GetYaw()) * cos(glm::radians(GetPitch())));
    front.y = sin(glm::radians(GetPitch()));
    front.z = sin(glm::radians(GetYaw()) * cos(glm::radians(GetPitch())));
    data_.front = glm::normalize(front);
    data_.right = glm::normalize(glm::cross(data_.front, data_.up));
    data_.up = glm::normalize(glm::cross(data_.right, data_.front));
  }

  void PerspectiveCamera::OnKeyPressed(const keyboard::KeyCode code) {
    const auto engine = engine::Engine::GetEngine();
    MCC_ASSERT(engine);
    const auto& tick = ((engine::TickState*) engine->GetCurrentState())->GetCurrentTick();
    const auto velocity = data_.CalculateVelocity(tick.delta);
  }

  void PerspectiveCamera::OnMouseMoved(const mouse::MouseMoveEvent* event) {

  }

  std::string PerspectiveCamera::ToString() const {
    std::stringstream ss;
    ss << "PerspectiveCamera(";
    ss << "data=" << data_;
    ss << ")";
    return ss.str();
  }

  static ThreadLocal<PerspectiveCamera> camera_;

  static inline PerspectiveCamera*
  CreatePerspectiveCamera() {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }

  PerspectiveCamera* GetPerspectiveCamera() {
    if(camera_)
      return camera_.Get();
    const auto camera = CreatePerspectiveCamera();
    camera_.Set(camera);
    return camera;
  }
}