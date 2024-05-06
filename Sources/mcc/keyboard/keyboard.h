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
    rx::observable<KeyboardEvent*> OnKeyboardEvent();
#define DEFINE_ON_KEYBOARD_EVENT(Name)                  \
    static inline rx::observable<Name##Event*>          \
    On##Name##Event() {                                 \
      return OnKeyboardEvent()                          \
        .filter(Name##Event::Filter)                    \
        .map(Name##Event::Cast);                        \
    }
    FOR_EACH_KEYBOARD_EVENT(DEFINE_ON_KEYBOARD_EVENT)
#undef DEFINE_ON_KEYBOARD_EVENT

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
  }

  using keyboard::Keyboard;
  using keyboard::GetKeyboard;
}

#include "mcc/keyboard/keyboard_glfw.h"

#endif //MCC_KEYBOARD_H