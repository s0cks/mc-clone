#include <vector>
#include <array>
#include <queue>
#include "mcc/mouse.h"

namespace mcc {
  typedef std::array<MouseButtonListenerList, kNumberOfMouseButtonStates> MouseButtonStateListeners;
  typedef std::array<MouseButtonStateListeners, kNumberOfMouseButtonStates> MouseButtonsStateListeners;
  
  static MousePositionCallbackList pos_listeners_;
  static MouseButtonsStateListeners listeners_;
  static std::queue<MouseButtonSubscription> removed_;
  static bool state[] = {
    false,
    false,
  };
  static glm::vec2 pos = glm::vec2(0,0);
  static glm::vec2 delta = glm::vec2(0, 0);

  static const MouseEventId kMousePressedEventId = Murmur("Event::Mouse::Pressed");
  static const MouseEventId kMouseReleasedEventId = Murmur("Event::Mouse::Released");

  static inline void 
  OnMouseInput(GLFWwindow* window, int btn, int action, int mods) {
    if(action == kMousePressed) {
      state[btn] = true;
    } else {
      state[btn] = false;
    }
    const auto& listeners = listeners_[btn][action];
    std::for_each(listeners.begin(), listeners.end(), [](const MouseButtonListener& listener) {
      listener();
    });
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

  MousePositionSubscription Mouse::Register(MousePositionCallback callback) {
    pos_listeners_.push_back(callback);
    return kInvalidMousePositionSubscription; //TODO: implement
  }
  
  void Mouse::DeregisterPositionSubscription(const MousePositionSubscription subscription) {
    //TODO: implement
  }

  MouseButtonSubscription Mouse::Register(const MouseButton btn, const MouseButtonState state, MouseButtonListener listener) {
    listeners_[btn][state].push_back(listener);
    return kInvalidMouseButtonSubscription; //TODO: implement
  }

  void Mouse::DeregisterButtonSubscription(const MouseButtonSubscription subscription) {
    //TODO: implement
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

#define DEFINE_SEND_EVENT(Name) \
  void Mouse::Send##Name##Event() { Events::Send(kMouse##Name##EventId); }
  FOR_EACH_MOUSE_EVENT(DEFINE_SEND_EVENT)
#undef DEFINE_SEND_EVENT

#define DEFINE_REGISTER_LISTENER(Name) \
  void Mouse::Register##Name##EventListener(EventListener listener) { Events::Register(kMouse##Name##EventId, listener); }
  FOR_EACH_MOUSE_EVENT(DEFINE_REGISTER_LISTENER)
#undef DEFINE_REGISTER_LISTENER
}