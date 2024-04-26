#ifndef MCC_GUI_EVENTS_H
#define MCC_GUI_EVENTS_H

#include "mcc/event.h"
#include "mcc/common.h"

#include "mcc/gui/gui_constants.h"

namespace mcc::gui {
#define FOR_EACH_GUI_EVENT(V)       \
  V(RootComponentRegistered)        \
  V(RootComponentRemoved)           \
  V(WindowOpened)                   \
  V(WindowClosed)                   \
  V(MouseEnter)                     \
  V(MouseExit)

  class GuiEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_GUI_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class GuiEvent : public Event {
  protected:
    GuiEvent() = default;
  public:
    ~GuiEvent() override = default;

    virtual bool IsWindowEvent() const {
      return false;
    }
    
    virtual bool IsMouseEvent() const {
      return false;
    }

#define DEFINE_TYPE_CHECK(Name)                                               \
    virtual Name##Event* As##Name##Event() { return nullptr; }                \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_GUI_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_GUI_EVENT(Name)                                               \
  public:                                                                     \
    std::string ToString() const override;                                    \
    const char* GetName() const override { return #Name; }                    \
    Name##Event* As##Name##Event() override { return this; }                  \
    static inline Name##Event*                                                \
    Cast(GuiEvent* event) {                                                   \
      MCC_ASSERT(event);                                                      \
      MCC_ASSERT(event->Is##Name##Event());                                   \
      return event->As##Name##Event();                                        \
    }                                                                         \
    static inline bool                                                        \
    Filter(GuiEvent* event) {                                                 \
      return event                                                            \
          && event->Is##Name##Event();                                        \
    }
  
  class Component;
  class RootComponentEvent : public GuiEvent {
  protected:
    const Component* component_;

    explicit RootComponentEvent(const Component* component):
      GuiEvent(),
      component_(component) {
      MCC_ASSERT(component);
    }
  public:
    ~RootComponentEvent() override = default;

    const Component* GetComponent() const {
      return component_;
    }
  };
  
  class RootComponentRegisteredEvent : public RootComponentEvent {
  public:
    explicit RootComponentRegisteredEvent(const Component* component):
      RootComponentEvent(component) {
    }
    ~RootComponentRegisteredEvent() override = default;
    DECLARE_GUI_EVENT(RootComponentRegistered);
  };

  class RootComponentRemovedEvent : public RootComponentEvent {
  public:
    explicit RootComponentRemovedEvent(const Component* component):
      RootComponentEvent(component) {
    }
    ~RootComponentRemovedEvent() override = default;
    DECLARE_GUI_EVENT(RootComponentRemoved);
  };

  class Window;
  class WindowEvent : public GuiEvent {
  protected:
    const Window* window_;

    explicit WindowEvent(const Window* window):
      GuiEvent(),
      window_(window) {
    }
  public:
    ~WindowEvent() override = default;

    bool IsWindowEvent() const override {
      return true;
    }

    const Window* GetWindow() const {
      return window_;
    }
  };

  class WindowOpenedEvent : public WindowEvent {
  public:
    explicit WindowOpenedEvent(const Window* window):
      WindowEvent(window) {
    }
    ~WindowOpenedEvent() override = default;
    DECLARE_GUI_EVENT(WindowOpened);
  };

  class WindowClosedEvent : public WindowEvent {
  public:
    explicit WindowClosedEvent(const Window* window):
      WindowEvent(window) {
    }
    ~WindowClosedEvent() override = default;
    DECLARE_GUI_EVENT(WindowClosed);
  };

  class MouseEvent : public GuiEvent {
  protected:
    Point pos_;

    explicit MouseEvent(const Point& pos):
      GuiEvent(),
      pos_(pos) {
    }
  public:
    ~MouseEvent() override = default;

    bool IsMouseEvent() const override {
      return true;
    }

    const Point& GetPos() const {
      return pos_;
    }
  };

  class MouseEnterEvent : public MouseEvent {
  public:
    explicit MouseEnterEvent(const Point& pos):
      MouseEvent(pos) {
    }
    ~MouseEnterEvent() override = default;
    DECLARE_GUI_EVENT(MouseEnter);
  };

  class MouseExitEvent : public MouseEvent {
  public:
    explicit MouseExitEvent(const Point& pos):
      MouseEvent(pos) {
    }
    ~MouseExitEvent() override = default;
    DECLARE_GUI_EVENT(MouseExit);
  };
}

#endif //MCC_GUI_EVENTS_H