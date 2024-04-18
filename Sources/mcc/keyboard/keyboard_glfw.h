#ifndef MCC_KEYBOARD_H
#error "Please #include <mcc/keyboard/keyboard.h> instead."
#endif //MCC_KEYBOARD_H

#ifndef MCC_KEYBOARD_GLFW_H
#define MCC_KEYBOARD_GLFW_H

#include "mcc/engine/engine.h"
#include "mcc/window/window.h"
#include "mcc/keyboard/keyboard.h"

namespace mcc::keyboard {
  class GlfwKeyboard : public Keyboard {
  protected:
    Window* window_;
    rx::subject<KeyboardEvent*> events_;
    rx::subscription pre_tick_sub_;

    inline Window* window() const {
      return window_;
    }

    void Process() override;
  public:
    explicit GlfwKeyboard(engine::Engine* engine, Window* window):
      Keyboard(),
      window_(window),
      events_(),
      pre_tick_sub_() {
      pre_tick_sub_ = engine->OnPreTickEvent().subscribe([this](engine::PreTickEvent* event) {
        Process();
      });
    }
    ~GlfwKeyboard() override = default;

    rx::observable<KeyboardEvent*> OnEvent() const override;
    KeyState GetKey(const KeyCode code) const override;
  };
}

#endif //MCC_KEYBOARD_GLFW_H