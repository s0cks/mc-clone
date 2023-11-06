#include "mcc/input/keyboard.h"
#include "mcc/window.h"

namespace mcc {
  static KeyEventSubject events_;
  static InputState keys[kNumberOfKeyCodes];

  void Keyboard::Process() {
    const auto window = Window::GetHandle();
    auto subscriber = events_.get_subscriber();
    for(auto idx = 0; idx < kNumberOfKeyCodes; idx++) {
      const auto code = static_cast<KeyCode>(idx);
      if(code == kKeyUnknown)
        continue;
      const auto last_state = keys[idx];
      keys[idx] = static_cast<InputState>(glfwGetKey(window, FromKeyCode(code)));
      if(last_state != keys[idx])
        subscriber.on_next(KeyEvent {
          .code = code,
          .state = keys[idx],
        });
    }
  }

  rx::observable<KeyEvent> Keyboard::GetKeyEventObservable() {
    return events_.get_observable();
  }
}