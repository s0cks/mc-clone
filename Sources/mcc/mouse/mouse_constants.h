#ifndef MCC_MOUSE_CONSTANTS_H
#define MCC_MOUSE_CONSTANTS_H

#include <iostream>
#include "mcc/gfx.h"

namespace mcc::mouse {
  struct MousePosition {
    glm::vec2 pos;
    glm::vec2 delta;

    friend std::ostream& operator<<(std::ostream& stream, const MousePosition& rhs) {
      stream << "MousePosition(";
      stream << "pos=" << glm::to_string(rhs.pos) << ", ";
      stream << "delta=" << glm::to_string(rhs.delta);
      stream << ")";
      return stream;
    }
  };

  enum MouseButtonState : uint8_t {
    kMousePressed = GLFW_PRESS,
    kMouseReleased = GLFW_RELEASE,
    kMouseRepeat = GLFW_REPEAT,
  };

  enum MouseButton : uint8_t {
    kButton1 = GLFW_MOUSE_BUTTON_1,
    kButton2 = GLFW_MOUSE_BUTTON_2,
    kButton3 = GLFW_MOUSE_BUTTON_3,
    kButton4 = GLFW_MOUSE_BUTTON_4,
    kButton5 = GLFW_MOUSE_BUTTON_5,
    kButton6 = GLFW_MOUSE_BUTTON_6,
    kButton7 = GLFW_MOUSE_BUTTON_7,
    kButton8 = GLFW_MOUSE_BUTTON_8,
    kNumberOfMouseButtons = (GLFW_MOUSE_BUTTON_LAST + 1),
    kLeftButton = GLFW_MOUSE_BUTTON_LEFT,
    kRightButton = GLFW_MOUSE_BUTTON_RIGHT,
    kMiddleButton = GLFW_MOUSE_BUTTON_MIDDLE,
  };
}

#endif //MCC_MOUSE_CONSTANTS_H