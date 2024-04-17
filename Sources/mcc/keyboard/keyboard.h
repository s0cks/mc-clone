#ifndef MCC_KEYBOARD_H
#define MCC_KEYBOARD_H

#include "mcc/rx.h"
#include "mcc/gfx.h"
#include "mcc/keyboard/keyboard_event.h"
#include "mcc/keyboard/keyboard_constants.h"

namespace mcc {
  namespace engine {
    class Engine;
  }

  namespace keyboard {
    class Keyboard {
      friend class engine::Engine;
    protected:
      Keyboard() = default;
      virtual void Process() = 0;
    public:
      virtual ~Keyboard() = default;
      virtual rx::observable<KeyboardEvent*> OnEvent() const = 0;
      virtual KeyState GetKey(const KeyCode code) const = 0;

      inline bool IsPressed(const KeyCode code) const {
        return GetKey(code) == kKeyPressed;
      }

      inline bool IsReleased(const KeyCode code) const {
        return GetKey(code) == kKeyReleased;
      }

      inline rx::observable<KeyPressedEvent*> OnPressed() const {
        return OnEvent()
          .filter(KeyPressedEvent::FilterBy)
          .map(KeyPressedEvent::Cast);
      }

      inline rx::observable<KeyPressedEvent*> OnPressed(const KeyCode code) const {
        return OnEvent()
          .filter(KeyPressedEvent::FilterByCode(code))
          .map(KeyPressedEvent::Cast);
      }

      inline rx::observable<KeyReleasedEvent*> OnReleased() const {
        return OnEvent()
          .filter(KeyReleasedEvent::FilterBy)
          .map(KeyReleasedEvent::Cast);
      }

      inline rx::observable<KeyReleasedEvent*> OnReleased(const KeyCode code) const {
        return OnEvent()
          .filter(KeyReleasedEvent::FilterByCode(code))
          .map(KeyReleasedEvent::Cast);
      }
    };

    void InitKeyboard();
    bool HasKeyboard();
    Keyboard* GetKeyboard();
    rx::observable<KeyboardEvent*> OnEvent();

    static inline rx::observable<KeyboardCreatedEvent*>
    OnCreated() {
      return OnEvent()
        .filter(KeyboardCreatedEvent::FilterBy)
        .map(KeyboardCreatedEvent::Cast);
    }

    static inline rx::observable<KeyboardDestroyedEvent*>
    OnDestroyed() {
      return OnEvent()
        .filter(KeyboardDestroyedEvent::FilterBy)
        .map(KeyboardDestroyedEvent::Cast);
    }
  }

  using keyboard::Keyboard;
  using keyboard::GetKeyboard;
}

#include "mcc/keyboard/keyboard_glfw.h"

#endif //MCC_KEYBOARD_H