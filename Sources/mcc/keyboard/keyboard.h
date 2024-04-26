#ifndef MCC_KEYBOARD_H
#define MCC_KEYBOARD_H

#include "mcc/rx.h"
#include "mcc/gfx.h"
#include "mcc/keyboard/key_state.h"
#include "mcc/keyboard/keyboard_event.h"

#include "mcc/engine/engine_event_listener.h"

namespace mcc {
  namespace engine {
    class Engine;
  }

  namespace keyboard {
    class Keyboard : public engine::PreTickEventListener {
      friend class engine::Engine;
    protected:
      static void PublishEvent(KeyboardEvent* event);
    protected:
      explicit Keyboard(engine::Engine* engine):
        PreTickEventListener(engine) {  
      }

      template<class E, typename... Args>
      inline void Publish(Args... args) const {
        E event(this, args...);
        return PublishEvent(&event);
      }
    public:
      virtual ~Keyboard() = default;
      rx::observable<KeyboardEvent*> OnEvent() const;
      virtual KeyState GetKey(const KeyCode code) const = 0;

      inline bool IsPressed(const KeyCode code) const {
        return GetKey(code) == kKeyPressed;
      }

      inline bool IsReleased(const KeyCode code) const {
        return GetKey(code) == kKeyReleased;
      }

      inline rx::observable<KeyPressedEvent*> OnPressed() const {
        return OnEvent()
          .filter(KeyPressedEvent::Filter)
          .map(KeyPressedEvent::Cast);
      }

      inline rx::observable<KeyPressedEvent*> OnPressed(const KeyCode code) const {
        return OnEvent()
          .filter(KeyPressedEvent::FilterBy(code))
          .map(KeyPressedEvent::Cast);
      }

      inline rx::observable<KeyReleasedEvent*> OnReleased() const {
        return OnEvent()
          .filter(KeyReleasedEvent::Filter)
          .map(KeyReleasedEvent::Cast);
      }

      inline rx::observable<KeyReleasedEvent*> OnReleased(const KeyCode code) const {
        return OnEvent()
          .filter(KeyReleasedEvent::FilterBy(code))
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
        .filter(KeyboardCreatedEvent::Filter)
        .map(KeyboardCreatedEvent::Cast);
    }

    static inline rx::observable<KeyboardDestroyedEvent*>
    OnDestroyed() {
      return OnEvent()
        .filter(KeyboardDestroyedEvent::Filter)
        .map(KeyboardDestroyedEvent::Cast);
    }
  }

  using keyboard::Keyboard;
  using keyboard::GetKeyboard;
}

#include "mcc/keyboard/keyboard_glfw.h"

#endif //MCC_KEYBOARD_H