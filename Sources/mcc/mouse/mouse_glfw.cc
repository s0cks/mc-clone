#include "mcc/mouse/mouse.h"

#include "mcc/window/window.h"
#include "mcc/camera/perspective_camera.h"
#include "mcc/physics/transform.h"

namespace mcc::mouse {
  GlfwMouse::GlfwMouse(engine::Engine* engine, Window* window):
    Mouse(),
    engine_(engine),
    window_(window),
    pos_(),
    last_pos_(),
    delta_(),
    buttons_(),
    events_(),
    pre_tick_sub_() {
    pre_tick_sub_ = engine->OnPreTickEvent().subscribe([this](engine::PreTickEvent* event) {
      Process();
    });
  }

  std::optional<Entity> GlfwMouse::CastRayTo(const float diff) const {
    std::optional<Entity> result = std::nullopt;
    const auto ray = CastRay();
    DLOG(INFO) << "ray: " << glm::to_string(ray);
    physics::Transform::Visit([&result,&ray,&diff](const Entity& e, const ComponentState<physics::Transform>& transform) {
      const auto& pos = (*transform).position;
      DLOG(INFO) << e << " pos: " << glm::to_string(pos);
      const auto distance = ray - pos;
      DLOG(INFO) << "distance: " << glm::to_string(distance);
      if(abs(distance.x) <= diff && abs(distance.y) <= diff && abs(distance.z) <= diff) {
        result = std::optional<Entity>{ e };
        return false;
      }
      return true;
    });
    return result;
  }

  glm::vec3 GlfwMouse::CastRay() const {
    auto ndc = GetNormalizedPosition();
    const auto camera = camera::PerspectiveCameraBehavior::GetCameraComponent();
    const auto projection = (*camera)->GetProjectionMatrix();
    const auto view = (*camera)->GetViewMatrix();
    const auto screen_pos = glm::vec4(ndc.x, -ndc.y, 1.0f, 1.0f);
    const auto inverse_vp = glm::inverse(projection * view);
    const auto world_pos = inverse_vp * screen_pos;
    return glm::normalize(glm::vec3(world_pos));
  }

  glm::vec2 GlfwMouse::GetNormalizedPosition() const {
    const auto size = Window::Get()->GetSize();
    return glm::vec2(2.0f * pos_.x / size[0] - 1.0f, 1.0f - 2.0f * pos_.y / size[1]);
  }

  glm::vec2 GlfwMouse::GetCursorPosition() const {
    const auto window = Window::Get();
    double xPos, yPos;
    glfwGetCursorPos(window->handle(), &xPos, &yPos);
    return glm::vec2(xPos, yPos);
  }
  
  static inline bool
  HasNoMotion(const glm::vec2& delta) {
    return delta[0] == 0.0f && delta[1] == 0.0f;
  }

  static inline bool
  HasMotion(const glm::vec2& delta) {
    return !HasNoMotion(delta);
  }

  void GlfwMouse::Process() {
    const auto handle = Window::Get()->handle();
    pos_ = GetCursorPosition();
    delta_ = (pos_ - last_pos_);
    if(HasMotion(delta_)) {
      Publish<MouseMoveEvent>(pos_, last_pos_, delta_);
    }
    last_pos_ = pos_;

    for(auto idx = 0; idx < kNumberOfMouseButtons; idx++) {
      const auto btn = static_cast<MouseButton>(idx);
      const auto& last_state = buttons_[btn];
      const auto new_state = GetButton(btn);
      if(new_state != last_state)
        Publish<MouseButtonEvent>(btn, last_state, new_state);
      buttons_[btn] = new_state;
    }
  }

  MouseButtonState GlfwMouse::GetButton(const MouseButton btn) const {
    return static_cast<MouseButtonState>(glfwGetMouseButton(window()->handle(), btn));
  }
}