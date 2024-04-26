#include "mcc/keyboard/key.h"
#include "mcc/common.h"
#include "mcc/keyboard/keyboard.h"

namespace mcc::keyboard {
  KeyState Key::GetState(GLFWwindow* window) const {
    MCC_ASSERT(window);
    return static_cast<KeyState>(glfwGetKey(window, GetCode()));
  }

  KeySet::KeySet(const Keyboard* owner):
    owner_(owner),
    keys_() {
    MCC_ASSERT(owner);
    Insert(GLFW_KEY_SPACE);
    Insert(GLFW_KEY_APOSTROPHE);
    Insert(GLFW_KEY_COMMA);
    Insert(GLFW_KEY_MINUS);
    Insert(GLFW_KEY_PERIOD);
    Insert(GLFW_KEY_SLASH);
    Insert(GLFW_KEY_0);
  }

  GLFWwindow* KeySet::GetWindowHandle() const {
    return ((GlfwKeyboard*) GetOwner())->GetWindowHandle();
  }
}