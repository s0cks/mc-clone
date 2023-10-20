#ifndef MCC_MOUSE_H
#define MCC_MOUSE_H

#include "mcc/gfx.h"
#include "mcc/common.h"

namespace mcc {
  enum MouseAction {
    kMousePressed = GLFW_PRESS,
    kMouseReleased = GLFW_RELEASE,
  };

  struct MouseButton {
    bool down;
  };

  class Mouse {
  public:
    DEFINE_NON_INSTANTIABLE_TYPE(Mouse);
    static void Initialize(GLFWwindow* window);
    static bool IsPressed(int button);
    static glm::vec2 GetPosition();
    static glm::vec2 GetDelta();
  };
}

#endif //MCC_MOUSE_H