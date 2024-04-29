#ifndef MCC_WINDOW_EVENTS_H
#define MCC_WINDOW_EVENTS_H

#include "mcc/event.h"
#include "mcc/window/window_constants.h"

namespace mcc {
  class Window;
  class WindowEvent;

#define FOR_EACH_WINDOW_EVENT(V)   \
  V(WindowOpened)                  \
  V(WindowClosed)                  \
  V(WindowPos)                     \
  V(WindowSize)                    \
  V(WindowFocus)                   \
  V(WindowIconify)                 \
  V(WindowRefresh)                 \
  V(WindowMaximize)                \
  V(WindowContentScale)

#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_WINDOW_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class WindowEvent : public Event {
    DEFINE_NON_COPYABLE_TYPE(WindowEvent);
  protected:
    Window* window_;
  public:
    WindowEvent() = delete;
    explicit WindowEvent(Window* window):
      window_(window) {
    }
    ~WindowEvent() override = default;

    Window* window() const {
      return window_;
    }

#define DEFINE_TYPE_CHECK(Name)                                               \
    virtual Name##Event* As##Name##Event() { return nullptr; }                \
    virtual bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_WINDOW_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_WINDOW_EVENT(Name)                                                                  \
    DEFINE_NON_COPYABLE_TYPE(Name##Event);                                                          \
  public:                                                                                           \
    Name##Event() = delete;                                                                         \
    const char* GetName() const override { return #Name; }                                          \
    std::string ToString() const override;                                                          \
    Name##Event* As##Name##Event() override { return this; }                                        \
    static inline bool                                                                              \
    Filter(WindowEvent* event) {                                                                    \
      return event                                                                                  \
          && event->Is##Name##Event();                                                              \
    }                                                                                               \
    static inline Name##Event*                                                                      \
    Cast(WindowEvent* event) {                                                                      \
      MCC_ASSERT(event);                                                                            \
      MCC_ASSERT(event->Is##Name##Event());                                                         \
      return event->As##Name##Event();                                                              \
    }

  class WindowOpenedEvent : public WindowEvent {
  public:
    explicit WindowOpenedEvent(Window* window):
      WindowEvent(window) {
    }
    ~WindowOpenedEvent() override = default;
    DECLARE_WINDOW_EVENT(WindowOpened);
  };

  class WindowClosedEvent : public WindowEvent {
  public:
    explicit WindowClosedEvent(Window* window):
      WindowEvent(window) {
    }
    ~WindowClosedEvent() override = default;
    DECLARE_WINDOW_EVENT(WindowClosed);
  };

  class WindowPosEvent : public WindowEvent {
  protected:
    WindowPos pos_;
  public:
    WindowPosEvent(Window* window, const WindowPos& pos):
      WindowEvent(window),
      pos_(pos) {
    }
    explicit WindowPosEvent(Window* window, const uint32_t x = 0, const uint32_t y = 0):
      WindowPosEvent(window, { x, y }) {
    }
    ~WindowPosEvent() override = default;

    WindowPos pos() const {
      return pos_;
    }

    inline uint32_t x() const {
      return pos_[0];
    }

    inline uint32_t y() const {
      return pos_[1];
    }

    DECLARE_WINDOW_EVENT(WindowPos);
  };

  class WindowSizeEvent : public WindowEvent {
  private:
    WindowSize size_;
  public:
    WindowSizeEvent(Window* window, const WindowSize& size):
      WindowEvent(window),
      size_(size) {
    }
    WindowSizeEvent(Window* window, const uint32_t width, const uint32_t height):
      WindowSizeEvent(window, { width, height }) {
    }
    explicit WindowSizeEvent(Window* window):
      WindowSizeEvent(window, 0, 0) {
    }
    ~WindowSizeEvent() override = default;
  
    WindowSize size() const {
      return size_;
    }

    inline uint32_t width() const {
      return size_[0];
    }

    inline uint32_t height() const {
      return size_[1];
    }

    DECLARE_WINDOW_EVENT(WindowSize);
  };

  class WindowFocusEvent : public WindowEvent {
  private:
    bool focused_;
  public:
    explicit WindowFocusEvent(Window* window, bool focused):
      WindowEvent(window),
      focused_(focused) {
    }
    ~WindowFocusEvent() override = default;

    bool focused() const {
      return focused_;
    }

    DECLARE_WINDOW_EVENT(WindowFocus);
  };

  class WindowIconifyEvent : public WindowEvent {
  private:
    bool iconified_;
  public:
    WindowIconifyEvent(Window* window, bool iconified):
      WindowEvent(window),
      iconified_(iconified) {
    }
    ~WindowIconifyEvent() override = default;

    bool iconified() const {
      return iconified_;
    }

    DECLARE_WINDOW_EVENT(WindowIconify);
  };

  class WindowRefreshEvent : public WindowEvent {
  public:
    WindowRefreshEvent(Window* window):
      WindowEvent(window) {
    }
    ~WindowRefreshEvent() = default;
    DECLARE_WINDOW_EVENT(WindowRefresh);
  };

  class WindowMaximizeEvent : public WindowEvent {
  private:
    bool maximized_;
  public:
    WindowMaximizeEvent(Window* window, const bool maximized):
      WindowEvent(window),
      maximized_(maximized) {
    }
    ~WindowMaximizeEvent() override = default;

    bool maximized() const {
      return maximized_;
    }

    DECLARE_WINDOW_EVENT(WindowMaximize);
  };

  class WindowContentScaleEvent : public WindowEvent {
  private:
    glm::vec2 scale_;
  public:
    WindowContentScaleEvent(Window* window, const glm::vec2 scale):
      WindowEvent(window),
      scale_(scale) {
    }
    WindowContentScaleEvent(Window* window, const float xScale, const float yScale):
      WindowContentScaleEvent(window, glm::vec2(xScale, yScale)) {
    }
    ~WindowContentScaleEvent() override = default;

    glm::vec2 scale() const {
      return scale_;
    }

    DECLARE_WINDOW_EVENT(WindowContentScale);
  };
}

#endif //MCC_WINDOW_EVENTS_H