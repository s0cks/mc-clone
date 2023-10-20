#include "mcc/mouse.h"

namespace mcc {
  static bool state[] = {
    false,
    false,
  };
  static glm::vec2 pos = glm::vec2(0,0);
  static glm::vec2 delta = glm::vec2(0, 0);

  static inline void 
  OnMouseInput(GLFWwindow* window, int button, int action, int mods) {
    state[button] = (action == MouseAction::kMousePressed);
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
    delta = p - pos;
    delta = glm::vec2(Clamp(delta[0], -100.0f, 100.0f), Clamp(delta[1], -100.0f, 100.0f));
    pos = p;
  }

  void Mouse::Initialize(GLFWwindow* window) {
    glfwSetMouseButtonCallback(window, &OnMouseInput);
    glfwSetCursorPosCallback(window, &OnCursor);
  }

  bool Mouse::IsPressed(int button) {
    return state[button];
  }

  glm::vec2 Mouse::GetPosition() {
    return glm::vec2(pos);
  }

  glm::vec2 Mouse::GetDelta() {
    return glm::vec2(delta);
  }
}