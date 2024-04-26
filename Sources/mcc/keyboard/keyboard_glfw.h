#ifndef MCC_KEYBOARD_H
#error "Please #include <mcc/keyboard/keyboard.h> instead."
#endif //MCC_KEYBOARD_H

#ifndef MCC_KEYBOARD_GLFW_H
#define MCC_KEYBOARD_GLFW_H

#include "mcc/engine/engine.h"
#include "mcc/window/window.h"


#include "mcc/keyboard/key.h"
#include "mcc/keyboard/keyboard.h"
#include "mcc/keyboard/key_state_set.h"

namespace mcc::keyboard {
  class GlfwKeyboard : public Keyboard {
    friend class KeySet;
  protected:
    Window* window_;
    KeySet keys_;
    KeyStateSet states_;

    inline Window* window() const {
      return window_;
    }

    inline GLFWwindow* GetWindowHandle() const {
      return window_->handle();
    }

    void OnPreTick(engine::PreTickEvent* event) override;
  public:
    explicit GlfwKeyboard(engine::Engine* engine, Window* window):
      Keyboard(engine),
      window_(window),
      keys_(this),
      states_(keys_) {
    }
    ~GlfwKeyboard() override = default;

    KeyState GetKey(const KeyCode code) const override;
  };
}

#endif //MCC_KEYBOARD_GLFW_H