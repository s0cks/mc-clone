#ifndef MCC_WINDOW_H
#error "Please #include <mcc/window/window.h> directly."
#endif //MCC_WINDOW_H

#ifndef MCC_WINDOW_GLFW_H
#define MCC_WINDOW_GLFW_H

#include "mcc/window/window_constants.h"

namespace mcc {
  typedef GLFWwindow WindowHandle;

  static inline void
  SetWindowHandleData(WindowHandle* handle, uword address) {
    glfwSetWindowUserPointer(handle, reinterpret_cast<void*>(address));
  }

  template<typename T>
  static inline void
  SetWindowHandleData(WindowHandle* handle, T* data) {
    return SetWindowHandleData(handle, reinterpret_cast<uword>(data));
  }

  static inline uword
  GetWindowHandleData(WindowHandle* handle) {
    return reinterpret_cast<uword>(glfwGetWindowUserPointer(handle));
  }

  template<typename T>
  static inline T*
  GetWindowHandleData(WindowHandle* handle) {
    return (T*) GetWindowHandleData(handle);
  }

  class Window;
  static Window* CreateWindow();
}

#endif //MCC_WINDOW_GLFW_H