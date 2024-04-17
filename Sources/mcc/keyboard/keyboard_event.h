#ifndef MCC_KEYBOARD_EVENT_H
#define MCC_KEYBOARD_EVENT_H

#include "mcc/event.h"
#include "mcc/common.h"
#include "mcc/keyboard/keyboard_constants.h"

namespace mcc::keyboard {
#define FOR_EACH_KEYBOARD_EVENT(V) \
  V(KeyboardCreated)               \
  V(KeyboardDestroyed)             \
  V(Key)                           \
  V(KeyPressed)                    \
  V(KeyReleased)

  class Keyboard;
  class KeyboardEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_KEYBOARD_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

#define DECLARE_KEYBOARD_EVENT(Name)                            \
  public:                                                       \
    const char* GetName() const override { return #Name; }      \
    std::string ToString() const override;                      \
  public:                                                       \
    static inline bool FilterBy(KeyboardEvent* event) {         \
      return event && event->Is##Name##Event();                 \
    }                                                           \
    static inline Name##Event* Cast(KeyboardEvent* event) {     \
      MCC_ASSERT(event && event->Is##Name##Event());            \
      return event->As##Name##Event();                          \
    }

  class KeyboardEvent : public Event {
  protected:
    Keyboard* keyboard_;

    explicit KeyboardEvent(Keyboard* keyboard):
      Event(),
      keyboard_(keyboard) {
    }
  public:
    ~KeyboardEvent() override = default;

    Keyboard* keyboard() const {
      return keyboard_;
    }

#define DEFINE_TYPE_CHECK(Name)                                         \
    virtual Name##Event* As##Name##Event() { return nullptr; }          \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_KEYBOARD_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

  class KeyboardCreatedEvent : public KeyboardEvent {
  public:
    explicit KeyboardCreatedEvent(Keyboard* keyboard):
      KeyboardEvent(keyboard) {
    }
    ~KeyboardCreatedEvent() override = default;
    DECLARE_KEYBOARD_EVENT(KeyboardCreated);
  };

  class KeyboardDestroyedEvent : public KeyboardEvent {
  public:
    explicit KeyboardDestroyedEvent(Keyboard* keyboard):
      KeyboardEvent(keyboard) {
    }
    ~KeyboardDestroyedEvent() override = default;
    DECLARE_KEYBOARD_EVENT(KeyboardDestroyed);
  };

  class KeyEvent : public KeyboardEvent {
  protected:
    KeyCode code_;
  public:
    KeyEvent(Keyboard* keyboard, const KeyCode code):
      KeyboardEvent(keyboard),
      code_(code) {
    }
    ~KeyEvent() override = default;

    KeyCode code() const {
      return code_;
    }

    DECLARE_KEYBOARD_EVENT(Key);
  };
  
  class KeyPressedEvent : public KeyEvent {
  public:
    KeyPressedEvent(Keyboard* keyboard, const KeyCode code):
      KeyEvent(keyboard, code) {
    }
    ~KeyPressedEvent() override = default;
    DECLARE_KEYBOARD_EVENT(KeyPressed);
  public:
    static inline std::function<bool(KeyboardEvent*)>
    FilterByCode(const KeyCode code) {
      return [code](KeyboardEvent* event) {
        return event
            && event->IsKeyPressedEvent()
            && (event->AsKeyPressedEvent()->code() == code);
      };
    }
  };

  class KeyReleasedEvent : public KeyEvent {
  public:
    KeyReleasedEvent(Keyboard* keyboard, const KeyCode code):
      KeyEvent(keyboard, code) {
    }
    ~KeyReleasedEvent() override = default;
    DECLARE_KEYBOARD_EVENT(KeyReleased);
  public:
    static inline std::function<bool(KeyboardEvent*)>
    FilterByCode(const KeyCode code) {
      return [code](KeyboardEvent* event) {
        return event
            && event->IsKeyReleasedEvent()
            && (event->AsKeyReleasedEvent()->code() == code);
      };
    }
  };
}

#endif //MCC_KEYBOARD_EVENT_H