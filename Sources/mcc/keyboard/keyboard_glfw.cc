#include "mcc/keyboard/keyboard.h"

#include "mcc/window/window.h"
#include "mcc/keyboard/key_state_set.h"

namespace mcc::keyboard {
  KeyState GlfwKeyboard::GetKey(const KeyCode code) const {
    const auto window = Window::Get();
    return static_cast<KeyState>(glfwGetKey(window->handle(), static_cast<int>(code)));
  }

  rx::observable<KeyboardEvent*> GlfwKeyboard::OnEvent() const {
    return events_.get_observable();
  }

  static inline void
  UpdateAll(Window* window, KeyStateSet& keys) {
    for(auto idx = 0; idx < kNumberOfKeyCodes; idx++) {
      const auto key = static_cast<KeyCode>(idx);
      const auto curr_state = keys.Get(key);
      const auto new_state = static_cast<KeyState>(glfwGetKey(window->handle(), key));
      keys.Set(key, new_state);
    }
  }

  void GlfwKeyboard::Process() {
    KeyStateSet keys;
    UpdateAll(window(), keys);
  }
}