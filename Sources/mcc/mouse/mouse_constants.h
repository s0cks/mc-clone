#ifndef MCC_MOUSE_CONSTANTS_H
#define MCC_MOUSE_CONSTANTS_H

#include <functional>
#include "mcc/gfx.h"

namespace mcc {
  typedef glm::vec2 MousePosition;

  typedef std::function<void(const MousePosition& pos)> MousePositionCallback;

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

  typedef std::function<void()> MouseButtonCallback;
}

#endif //MCC_MOUSE_CONSTANTS_H