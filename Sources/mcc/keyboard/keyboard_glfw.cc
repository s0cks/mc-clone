#include "mcc/keyboard/keyboard.h"

#include "mcc/window/window.h"
#include "mcc/keyboard/key_state_set.h"

namespace mcc::keyboard {
  KeyState GlfwKeyboard::GetKey(const KeyCode code) const {
    const auto window = Window::Get();
    return static_cast<KeyState>(glfwGetKey(window->handle(), static_cast<int>(code)));
  }

  void GlfwKeyboard::OnPreTick(engine::PreTickEvent* event) {
    //TODO: fix
    for(const auto& key : keys_) {
      const auto curr_state = states_.GetState(key).value_or(kKeyReleased);
      const auto new_state = keys_.GetState(key);
      if(curr_state == new_state)
        continue;

      states_.PutState(key, new_state);
      switch(new_state) {
        case kKeyPressed:
          Publish<KeyPressedEvent>(key);
          continue;
        case kKeyReleased:
          Publish<KeyReleasedEvent>(key);
          continue;
        default:
          continue;
      }
    }
  }
}