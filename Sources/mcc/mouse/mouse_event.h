#ifndef MCC_MOUSE_EVENT_H
#define MCC_MOUSE_EVENT_H

#include <iostream>

#include "mcc/rx.h"
#include "mcc/event.h"
#include "mcc/mouse/mouse_constants.h"

namespace mcc::mouse {
#define FOR_EACH_MOUSE_EVENT(V) \
  V(MouseButton)                \
  V(MouseMove)                  \
  V(MouseInitialized)           \
  V(MouseDeinitialized)

  class MouseEvent;
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
    std::string ToString() const override;

  class MouseInitializedEvent : public MouseEvent {
  public:
    explicit MouseInitializedEvent(Mouse* mouse):
      MouseEvent(mouse) {
    }
    ~MouseInitializedEvent() override = default;
    DECLARE_MOUSE_EVENT(MouseInitialized);

    friend std::ostream& operator<<(std::ostream& stream, const MouseInitializedEvent& rhs) {
      stream << "MouseInitializedEvent(";
      stream << "mouse=" << rhs.mouse();
      stream << ")";
      return stream;
    }
  };

  class MouseDeinitializedEvent : public MouseEvent {
  public:
    explicit MouseDeinitializedEvent(Mouse* mouse):
      MouseEvent(mouse) {
    }
    ~MouseDeinitializedEvent() override = default;
    DECLARE_MOUSE_EVENT(MouseDeinitialized);

    friend std::ostream& operator<<(std::ostream& stream, const MouseDeinitializedEvent& rhs) {
      stream << "MouseDeinitializedEvent(";
      stream << "mouse=" << rhs.mouse();
      stream << ")";
      return stream;
    }
  };

  class MouseButtonEvent : public MouseEvent {
  protected:
    MouseButton button_;
    MouseButtonState previous_;
    MouseButtonState current_;
  public:
    explicit MouseButtonEvent(Mouse* mouse,
                              const MouseButton button,
                              const MouseButtonState previous,
                              const MouseButtonState current):
      MouseEvent(mouse),
      button_(button),
      previous_(previous),
      current_(current) {
    }
    ~MouseButtonEvent() override = default;
    DECLARE_MOUSE_EVENT(MouseButton);

    MouseButton button() const {
      return button_;
    }

    MouseButtonState current() const {
      return current_;
    }

    MouseButtonState previous() const {
      return previous_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const MouseButtonEvent& rhs) {
      stream << "MouseButtonEvent(";
      stream << "mouse=" << rhs.mouse() << ", ";
      stream << "button=" << rhs.button() << ", ";
      stream << "current=" << rhs.current() << ", ";
      stream << "previous=" << rhs.previous();
      stream << ")";
      return stream;
    }
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
    DECLARE_MOUSE_EVENT(MouseMove);

    const glm::vec2& pos() const {
      return pos_;
    }

    const glm::vec2& last_pos() const {
      return last_pos_;
    }

    const glm::vec2& delta() const {
      return delta_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const MouseMoveEvent& rhs) {
      stream << "MouseMoveEvent(";
      stream << "mouse=" << rhs.mouse() << ", ";
      stream << "pos=" << glm::to_string(rhs.pos()) << ", ";
      stream << "last_pos=" << glm::to_string(rhs.last_pos()) << ", ";
      stream << "delta=" << glm::to_string(rhs.delta());
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_MOUSE_EVENT_H