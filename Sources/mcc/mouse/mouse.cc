#include "mcc/mouse/mouse.h"
#include "mcc/mouse/mouse_trie.h"

#include "mcc/window.h"
#include "mcc/camera/perspective_camera.h"

#include "mcc/physics/transform.h"

namespace mcc {
  static RootNode* root_;

  static inline void
  Call(const Mouse::Subscription subscription) {
    if(root_ != nullptr)
      return root_->Call(subscription);
  }

  static inline void
  Call(const Mouse::Subscription subscription, const glm::vec2 delta) {
    if(root_ != nullptr)
      return root_->Call(subscription, delta);
  }

  static bool state[] = {
    false,
    false,
  };
  static glm::vec2 pos = glm::vec2(0,0);
  static glm::vec2 delta = glm::vec2(0, 0);

  static inline void 
  OnMouseInput(GLFWwindow* window, int btn, int action, int mods) {
    if(action == kMousePressed) {
      state[btn] = true;
    } else {
      state[btn] = false;
    }
    const auto subscription = Mouse::Subscription::NewButtonSubscription(static_cast<MouseButton>(btn), static_cast<MouseButtonState>(action));
    Call(subscription);

    const auto e = Mouse::CastRayTo(0.25f);
    if(!e)
      return;
    DLOG(INFO) << "e: " << (*e);
  }

  template<typename T>
  static inline T
  Max(const T lhs, const T rhs) {
    return lhs > rhs ? lhs : rhs;
  }

  template<typename T>
  static inline T
  Min(const T lhs, const T rhs) {
    return lhs < rhs ? lhs : rhs;
  }

  template<typename T>
  static inline T
  Clamp(const T value, const T min, const T max) {
    return Max(min, Min(max, value));
  }

  static inline void
  OnCursor(GLFWwindow* window, double x, double y) {
    const auto p = glm::vec2(static_cast<float>(x), static_cast<float>(y));
    delta = glm::vec2(p.x - pos.x, pos.y - y);
    pos = p;
    const auto subscription = Mouse::Subscription::NewPositionSubscription();
    Call(subscription, delta);
  }

  void Mouse::Initialize(GLFWwindow* window) {
    glfwSetMouseButtonCallback(window, &OnMouseInput);
    glfwSetCursorPosCallback(window, &OnCursor);
    root_ = new RootNode();
  }

  Mouse::Subscription Mouse::Register(const uint32_t id, MousePositionCallback callback) {
    const auto subscription = Subscription::NewPositionSubscription(id);
    root_->Register(subscription, callback);
    return subscription;
  }

  Mouse::Subscription Mouse::Register(const uint32_t id, const MouseButton btn, const MouseButtonState state, MouseButtonCallback callback) {
    const auto subscription = Subscription::NewButtonSubscription(id, btn, state);
    root_->Register(subscription, callback);
    return subscription;
  }

  void Mouse::Deregister(const Subscription& subscription) {
    DLOG(INFO) << "deregistering " << subscription << "....";
    return root_->Deregister(subscription);
  }

  bool Mouse::IsPressed(const MouseButton button) {
    return state[button];
  }

  glm::vec2 Mouse::GetPosition() {
    return glm::vec2(pos);
  }

  glm::vec2 Mouse::GetNormalizedPosition() {
    const auto size = Window::GetSize();
    return glm::vec2(pos.x / (size[0] * 0.5f) - 1.0f, pos.y / (size[1] * 0.5f) - 1.0f);
  }

  glm::vec2 Mouse::GetDelta() {
    return glm::vec2(delta);
  }

  glm::vec3 Mouse::CastRay() {
    auto pos = GetNormalizedPosition();
    const auto camera = camera::PerspectiveCameraBehavior::GetCameraComponent();
    const auto projection = (*camera)->GetProjectionMatrix();
    const auto view = (*camera)->GetViewMatrix();
    const auto inverseVP = glm::inverse(view * projection);
    const auto screenPos = glm::vec4(pos.x, -pos.y, 1.0f, 1.0f);
    const auto worldPos = inverseVP * screenPos;
    return glm::normalize(glm::vec3(worldPos));
  }

  std::optional<Entity> Mouse::CastRayTo(const float diff) {
    std::optional<Entity> result = std::nullopt;
    const auto ray = Mouse::CastRay();
    DLOG(INFO) << "ray: " << glm::to_string(ray);
    physics::Transform::Visit([&result,&ray,&diff](const Entity& e, const ComponentState<physics::Transform>& transform) {
      const auto& pos = (*transform).position;
      if((ray.x - diff <= pos.x || pos.x <= ray.x + diff)
      && (ray.y - diff <= pos.y || pos.y <= ray.y + diff)
      && (ray.z - diff <= pos.z || pos.z <= ray.z + diff)) {
        result = std::optional<Entity>{ e };
        return false;
      }
      return true;
    });
    return result;
  }
}