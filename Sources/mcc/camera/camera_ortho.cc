#include "mcc/camera/camera_ortho.h"

#include "mcc/thread_local.h"
#include "mcc/window/window.h"

#include "mcc/keyboard/keyboard.h"

namespace mcc::camera {
  OrthoCamera::OrthoCamera(const glm::vec3& eye,
                           const glm::vec3& pos,
                           const glm::vec3& up,
                           const glm::vec2& viewport_size):
    Camera(),
    data_(viewport_size, eye, pos, up) {
    keyboard::OnEvent()
      .filter(keyboard::KeyPressedEvent::Filter)
      .map(keyboard::KeyPressedEvent::Cast)
      .subscribe([this](keyboard::KeyPressedEvent* event) {
        return OnKeyPressed(event->GetKeyCode());
      });
  }

  void OrthoCamera::MoveCamera(const glm::vec3& dir) {
    data_.pos += dir;
    data_.view = data_.CalculateView();
  }

  static constexpr const auto kLeft = glm::vec3(-1.0f, 0.0f, 0.0f);
  static constexpr const auto kRight = glm::vec3(1.0f, 0.0f, 0.0f);
  static constexpr const auto kUp = glm::vec3(0.0f, -1.0f, 0.0f);
  static constexpr const auto kDown = glm::vec3(0.0f, 1.0f, 0.0f);

  void OrthoCamera::OnKeyPressed(const keyboard::KeyCode code) {
    switch(code) {
      case GLFW_KEY_A:
        return MoveCamera(kLeft);
      case GLFW_KEY_D:
        return MoveCamera(kRight);
      case GLFW_KEY_W:
        return MoveCamera(kUp);
      case GLFW_KEY_S:
        return MoveCamera(kDown);
      default:
        break;
    }
  }

  static inline glm::vec2
  GetViewportSize() {
    const auto window = Window::Get();
    MCC_ASSERT(window);
    return window->GetSize();
  }

  OrthoCamera::OrthoCamera(const glm::vec3& eye,
                           const glm::vec3& pos,
                           const glm::vec3& up):
    OrthoCamera(eye, pos, up, GetViewportSize()) {
  }

  glm::mat4 OrthoCameraData::CalculateView() const {
    return glm::lookAt(pos + OrthoCamera::kFront, pos, up);
  }

  glm::mat4 OrthoCameraData::CalculateProjection() const {
    const auto window = Window::Get();
    MCC_ASSERT(window);
    const auto v = viewport[2] / viewport[3];
    return glm::ortho(-v, v, -1.0f, 1.0f, 0.1f, 100.0f);
  }

  glm::vec2 OrthoCamera::CalculateMouseNDC(const glm::vec3& pos, const glm::mat4& model) const {
    const auto rpos = glm::unProject(pos, model, data_.projection, data_.viewport);
    return glm::vec2(-rpos.x, -rpos.y);
  }
  
  glm::vec3 OrthoCamera::CastRayAt(const glm::vec2& mpos) const {
    const auto mX = mpos[0] / (data_.GetViewportWidth() * 0.5f) - 1.0f;
    const auto mY = mpos[1] / (data_.GetViewportHeight() * 0.5f) - 1.0f;
    const auto inverse_vp = glm::inverse(data_.projection * data_.view);
    const auto screen_pos = glm::vec4(mX, -mY, 1.0f, 1.0f);
    const auto world_pos = inverse_vp * screen_pos;
    return glm::normalize(glm::vec3(world_pos));
  }

  std::pair<glm::vec3, glm::vec3> OrthoCamera::CastRay(const glm::vec2& mpos, const float distance) const {
    const auto& start_pos = data_.pos;
    const auto dir = CastRayAt(mpos);
    const auto end_pos = start_pos + dir * distance;
    return std::make_pair(start_pos, end_pos);
  }

  static ThreadLocal<OrthoCamera> camera_;

  static inline bool
  HasOrthoCamera() {
    return camera_.Get() != nullptr;
  }

  void SetOrthoCamera(OrthoCamera* camera) {
    MCC_ASSERT(camera);
    camera_.Set(camera);
  }

  static inline OrthoCamera*
  CreateOrthoCamera() {
    DLOG(INFO) << "creating OrthoCamera.....";
    return new OrthoCamera();
  }

  OrthoCamera* GetOrthoCamera() {
    if(!HasOrthoCamera()) {
      const auto camera = CreateOrthoCamera();
      SetOrthoCamera(camera);
      return camera;
    }
    return camera_.Get();
  }
}