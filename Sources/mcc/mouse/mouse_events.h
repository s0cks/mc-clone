#ifndef MCC_MOUSE_EVENT_H
#define MCC_MOUSE_EVENT_H

#include <iostream>

#include "mcc/rx.h"
#include "mcc/event.h"

#include "mcc/mouse/mouse_button.h"

namespace mcc::mouse {
#define FOR_EACH_MOUSE_EVENT(V) \
  V(MouseCreated)               \
  V(MouseDestroyed)             \
  V(MouseMove)                  \
  V(MouseButtonPressed)         \
  V(MouseButtonReleased)

  class MouseEvent;
  class MouseButtonEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_MOUSE_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  typedef rx::subject<MouseEvent*> MouseEventSubject;
  typedef rx::observable<MouseEvent*> MouseEventObservable;

  class Mouse;
  class MouseEvent : public Event {
  protected:
    Mouse* mouse_;

    explicit MouseEvent(Mouse* mouse):
      Event(),
      mouse_(mouse) {
    }
  public:
    ~MouseEvent() override = default;

    Mouse* mouse() const {
      return mouse_;
    }

    virtual MouseButtonEvent* AsMouseButtonEvent() {
      return nullptr;
    }

    bool IsMouseButtonEvent() {
      return AsMouseButtonEvent() != nullptr;
    }

#define DEFINE_TYPE_CHECK(Name) \
    virtual Name##Event* As##Name##Event() { return nullptr; } \
    virtual bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_MOUSE_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK

    friend std::ostream& operator<<(std::ostream& stream, const MouseEvent& rhs) {
      stream << "MouseEvent(";
      stream << "mouse=" << rhs.mouse();
      stream << ")";
      return stream;
    }
  };

#define DECLARE_MOUSE_EVENT(Name)                                 \
  public:                                                         \
    Name##Event* As##Name##Event() override { return this; }      \
    const char* GetName() const override { return #Name; }        \
    std::string ToString() const override;                        \
    static inline bool                                            \
    Filter(MouseEvent* event) {                                   \
      return event                                                \
          && event->Is##Name##Event();                            \
    }                                                             \
    static inline Name##Event*                                    \
    Cast(MouseEvent* event) {                                     \
      MCC_ASSERT(event);                                          \
      MCC_ASSERT(event->Is##Name##Event());                       \
      return event->As##Name##Event();                            \
    }

  class MouseCreatedEvent : public MouseEvent {
  public:
    explicit MouseCreatedEvent(Mouse* mouse):
      MouseEvent(mouse) {
    }
    ~MouseCreatedEvent() override = default;
    DECLARE_MOUSE_EVENT(MouseCreated);
  };

  class MouseDestroyedEvent : public MouseEvent {
  public:
    explicit MouseDestroyedEvent(Mouse* mouse):
      MouseEvent(mouse) {
    }
    ~MouseDestroyedEvent() override = default;
    DECLARE_MOUSE_EVENT(MouseDestroyed);
  };

  class MouseButtonEvent : public MouseEvent {
  protected:
    MouseButton button_;
    MouseButton::State state_;
  public:
    MouseButtonEvent(Mouse* mouse,
                     const MouseButton button,
                     const MouseButton::State state):
      MouseEvent(mouse),
      button_(button),
      state_(state) {
    }
    ~MouseButtonEvent() override = default;

    MouseButton GetButton() const {
      return button_;
    }

    MouseButton::State GetState() const {
      return state_;
    }

#define DEFINE_STATE_CHECK(Name, Value)                                     \
    bool Is##Name() const { return GetState() == MouseButton::k##Name; }
    FOR_EACH_MOUSE_BUTTON_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK
  };

  class MouseButtonPressedEvent : public MouseButtonEvent {
  public:
    MouseButtonPressedEvent(Mouse* mouse, const MouseButton button):
      MouseButtonEvent(mouse, button, MouseButton::kPressed) {
    }
    ~MouseButtonPressedEvent() override = default;
    DECLARE_MOUSE_EVENT(MouseButtonPressed);
  };

  class MouseButtonReleasedEvent : public MouseButtonEvent {
  public:
    MouseButtonReleasedEvent(Mouse* mouse, const MouseButton button):
      MouseButtonEvent(mouse, button, MouseButton::kReleased) {
    }
    ~MouseButtonReleasedEvent() override = default;
    DECLARE_MOUSE_EVENT(MouseButtonReleased);
  };

  class MouseMoveEvent : public MouseEvent {
  protected:
    glm::vec2 pos_;
    glm::vec2 last_pos_;
    glm::vec2 delta_;
  public:
    explicit MouseMoveEvent(Mouse* mouse,
                            const glm::vec2 pos,
                            const glm::vec2 last_pos,
                            const glm::vec2 delta):
      MouseEvent(mouse),
      pos_(pos),
      last_pos_(last_pos),
      delta_(delta) {
    }
    ~MouseMoveEvent() override = default;

    const glm::vec2& pos() const {
      return pos_;
    }

    const glm::vec2& last_pos() const {
      return last_pos_;
    }

    const glm::vec2& delta() const {
      return delta_;
    }

    DECLARE_MOUSE_EVENT(MouseMove);
  };
}

#endif //MCC_MOUSE_EVENT_H