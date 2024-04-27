#include "mcc/mouse/mouse_button.h"

#include "mcc/window/window.h"

namespace mcc::mouse {
  MouseButton::State MouseButton::GetState(GLFWwindow* handle) const {
    MCC_ASSERT(handle);
    return static_cast<MouseButton::State>(glfwGetMouseButton(handle, GetCode()));
  }
}