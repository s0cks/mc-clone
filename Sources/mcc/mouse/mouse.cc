#include "mcc/mouse/mouse.h"
#include "mcc/mouse/mouse_trie.h"

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

  glm::vec2 Mouse::GetDelta() {
    return glm::vec2(delta);
  }
}