#include "mcc/input/mouse.h"
#include "mcc/window.h"
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
    const auto size = Window::GetSize();
    return glm::vec2((2.0f * pos.x) / size[0] - 1.0f, 1.0f - (2.0f * pos.y) / size[1]);
  }

  glm::vec2 Mouse::GetDelta() {
    return glm::vec2(delta);
  }

  glm::vec3 Mouse::CastRay() {
    auto pos = GetNormalizedPosition();
    const auto camera = camera::PerspectiveCameraBehavior::GetCameraComponent();
    const auto projection = (*camera)->GetProjectionMatrix();
    const auto view = (*camera)->GetViewMatrix();
    const auto ray_clip = glm::vec4(pos.x, pos.y, -1.0f, 1.0f);
    auto ray_eye = glm::inverse(projection) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
    const auto ray_world = glm::inverse(view) * ray_eye;
    return glm::normalize(glm::vec3(ray_world.x, ray_world.y, ray_world.z));
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
    const auto handle = Window::GetHandle();
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