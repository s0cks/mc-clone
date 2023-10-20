#ifndef MCC_MOUSE_H
#define MCC_MOUSE_H

#include <vector>
#include <functional>

#include "mcc/gfx.h"
#include "mcc/common.h"
#include "mcc/bitfield.h"
#include "mcc/ecs/event.h"

namespace mcc {
  typedef glm::vec2 MousePosition;

  typedef std::function<void(const MousePosition& pos)> MousePositionCallback;
  typedef std::vector<MousePositionCallback> MousePositionCallbackList;

  typedef uint32_t MousePositionSubscription;

  static constexpr const MousePositionSubscription kInvalidMousePositionSubscription = 0;

  enum MouseButtonState {
    kMousePressed = GLFW_PRESS,
    kMouseReleased = GLFW_RELEASE,
    kNumberOfMouseButtonStates = 2,
  };

  enum MouseButton {
    kMouseButton1 = GLFW_MOUSE_BUTTON_1,
    kMouseButton2 = GLFW_MOUSE_BUTTON_2,
    kNumberOfMouseButtons = 2,
  };

  typedef EventId MouseEventId;

#define FOR_EACH_MOUSE_EVENT(V) \
  V(Pressed)                    \
  V(Released)

  typedef std::function<void()> MouseButtonListener;
  typedef std::vector<MouseButtonListener> MouseButtonListenerList;

  typedef uint32_t MouseButtonSubscription;

  static constexpr const MouseButtonSubscription kInvalidMouseButtonSubscription = 0;

  class Mouse {
  public:
    static constexpr const MouseButtonSubscription kMaxNumberOfSubscriptions = UINT16_MAX;
  private:
#define DECLARE_SEND_EVENT(Name) \
    static void Send##Name##Event();
    FOR_EACH_MOUSE_EVENT(DECLARE_SEND_EVENT)
#undef DECLARE_SEND_EVENT
  public:
    DEFINE_NON_INSTANTIABLE_TYPE(Mouse);
    static void Initialize(GLFWwindow* window);
    static bool IsPressed(const MouseButton button);
    static glm::vec2 GetPosition();
    static glm::vec2 GetDelta();
    static MousePositionSubscription Register(MousePositionCallback callback);
    static void DeregisterPositionSubscription(const MousePositionSubscription subscription);
    static MouseButtonSubscription Register(const MouseButton btn, const MouseButtonState state, MouseButtonListener listener);
    static void DeregisterButtonSubscription(const MouseButtonSubscription subscription);

#define DECLARE_REGISTER_LISTENER(Name) \
    static void Register##Name##EventListener(EventListener listener);
    FOR_EACH_MOUSE_EVENT(DECLARE_REGISTER_LISTENER)
#undef DECLARE_REGISTER_LISTENER
  };
}

#endif //MCC_MOUSE_H