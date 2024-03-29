#include "mcc/input/mouse.h"
#include "mcc/window/window.h"
#include "mcc/camera/perspective_camera.h"
#include "mcc/physics/transform.h"

namespace mcc {
  static glm::dvec2 pos;
  static glm::dvec2 last_pos;
  static glm::dvec2 delta;
  static InputState buttons[kNumberOfMouseButtons];
  static rxsub::subject<MousePosition> position_;
  static MouseButtonEventSubject button_events_;

  glm::vec2 Mouse::GetPosition() {
    return glm::vec2(pos);
  }

  glm::vec2 Mouse::GetNormalizedPosition() {
    const auto size = Window::Get()->GetSize();
    return glm::vec2(2.0f * pos.x / size[0] - 1.0f, 1.0f - 2.0f * pos.y / size[1]);
  }

  glm::vec2 Mouse::GetDelta() {
    return glm::vec2(delta);
  }

  glm::vec3 Mouse::CastRay() {
    auto ndc = GetNormalizedPosition();
    const auto camera = camera::PerspectiveCameraBehavior::GetCameraComponent();
    const auto projection = (*camera)->GetProjectionMatrix();
    const auto view = (*camera)->GetViewMatrix();
    const auto screen_pos = glm::vec4(ndc.x, -ndc.y, 1.0f, 1.0f);
    const auto inverse_vp = glm::inverse(projection * view);
    const auto world_pos = inverse_vp * screen_pos;
    return glm::normalize(glm::vec3(world_pos));
  }

  std::optional<Entity> Mouse::CastRayTo(const float diff) {
    std::optional<Entity> result = std::nullopt;
    const auto ray = Mouse::CastRay();
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

  void Mouse::Process() {
    const auto handle = Window::Get()->handle();
    glfwGetCursorPos(handle, &pos[0], &pos[1]);
    delta = (pos - last_pos);
    position_.get_subscriber().on_next(MousePosition {
      .pos = pos,
      .delta = delta,
    });
    last_pos = pos;

    const auto subscriber = button_events_.get_subscriber();
    for(auto btn = 0; btn < kNumberOfMouseButtons; btn++) {
      const auto last_state = buttons[btn];
      buttons[btn] = static_cast<InputState>(glfwGetMouseButton(handle, btn));
      if(last_state != buttons[btn]) {
        subscriber.on_next(MouseButtonEvent {
          .button = static_cast<MouseButton>(btn),
          .state = buttons[btn],
        });
      }
    }
  }

  rxsub::subject<MousePosition>& Mouse::GetPositionSubject() {
    return position_;
  }

  rxsub::subject<MouseButtonEvent>& Mouse::GetButtonEventSubject() {
    return button_events_;
  }
}