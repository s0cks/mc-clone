#ifndef MCC_WINDOW_EVENT_H
#define MCC_WINDOW_EVENT_H

#include "mcc/window/window_constants.h"

namespace mcc {
  class Window;
  class WindowEvent;

#define FOR_EACH_WINDOW_EVENT(V)   \
  V(WindowOpenedEvent, opened)     \
  V(WindowClosedEvent, closed)     \
  V(WindowPosEvent, pos)           \
  V(WindowSizeEvent, size)         \
  V(WindowFocusEvent, focus)       \
  V(WindowIconifyEvent, iconify)   \
  V(WindowRefreshEvent, refresh)   \
  V(WindowMaximizeEvent, maximize) \
  V(WindowContentScaleEvent, content_scale)

#define FORWARD_DECLARE(Name, Subject) class Name;
  FOR_EACH_WINDOW_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class WindowEvent {
  protected:
    Window* window_;
  public:
    explicit WindowEvent(Window* window):
      window_(window) {
    }
    WindowEvent(const WindowEvent& rhs) = default;
    virtual ~WindowEvent() = default;

    Window* window() const {
      return window_;
    }

    WindowEvent& operator=(const WindowEvent& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const WindowEvent& rhs) {
      stream << "WindowEvent(";
      stream << "window=" << rhs.window();
      stream << ")";
      return stream;
    }
  };

  class WindowOpenedEvent : public WindowEvent {
  public:
    explicit WindowOpenedEvent(Window* window):
      WindowEvent(window) {
    }
    WindowOpenedEvent(const WindowOpenedEvent& rhs) = default;
    ~WindowOpenedEvent() override = default;

    WindowOpenedEvent& operator=(const WindowOpenedEvent& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const WindowOpenedEvent& rhs) {
      stream << "WindowOpenedEvent(";
      stream << "window=" << rhs.window();
      stream << ")";
      return stream;
    }
  };

  class WindowClosedEvent : public WindowEvent {
  public:
    explicit WindowClosedEvent(Window* window):
      WindowEvent(window) {
    }
    WindowClosedEvent(const WindowClosedEvent& rhs) = default;
    ~WindowClosedEvent() override = default;

    WindowClosedEvent& operator=(const WindowClosedEvent& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const WindowClosedEvent& rhs) {
      stream << "WindowClosedEvent(";
      stream << "window=" << rhs.window();
      stream << ")";
      return stream;
    }
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

    WindowPosEvent& operator=(const WindowPosEvent& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const WindowPosEvent& rhs) {
      stream << "WindowPosEvent(";
      stream << "window=" << rhs.window() << ", ";
      stream << "pos=" << glm::to_string(rhs.pos());
      stream << ")";
      return stream;
    }
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
    WindowSizeEvent(const WindowSizeEvent& rhs) = default;
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

    WindowSizeEvent& operator=(const WindowSizeEvent& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const WindowSizeEvent& rhs) {
      stream << "WindowSizeEvent(";
      stream << "window=" << rhs.window() << ", ";
      stream << "size=" << glm::to_string(rhs.size());
      stream << ")";
      return stream;
    }
  };

  class WindowFocusEvent : public WindowEvent {
  private:
    bool focused_;
  public:
    explicit WindowFocusEvent(Window* window, bool focused):
      WindowEvent(window),
      focused_(focused) {
    }
    WindowFocusEvent(const WindowFocusEvent& rhs) = default;
    ~WindowFocusEvent() override = default;

    bool focused() const {
      return focused_;
    }

    WindowFocusEvent& operator=(const WindowFocusEvent& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const WindowFocusEvent& rhs) {
      stream << "WindowFocusEvent(";
      stream << "window=" << rhs.window() << ", ";
      stream << "focus=" << (rhs.focused() ? "y" : "n");
      stream << ")";
      return stream;
    }
  };

  class WindowIconifyEvent : public WindowEvent {
  private:
    bool iconified_;
  public:
    WindowIconifyEvent(Window* window, bool iconified):
      WindowEvent(window),
      iconified_(iconified) {
    }
    WindowIconifyEvent(WindowIconifyEvent& rhs) = default;
    ~WindowIconifyEvent() override = default;

    bool iconified() const {
      return iconified_;
    }

    WindowIconifyEvent& operator=(const WindowIconifyEvent& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const WindowIconifyEvent& rhs) {
      stream << "WindowIconifyEvent(";
      stream << "window=" << rhs.window() << ", ";
      stream << "iconified=" << (rhs.iconified() ? "y" : "n");
      stream << ")";
      return stream;
    }
  };

  class WindowRefreshEvent : public WindowEvent {
  public:
    WindowRefreshEvent(Window* window):
      WindowEvent(window) {
    }
    WindowRefreshEvent(const WindowRefreshEvent& rhs) = default;
    ~WindowRefreshEvent() = default;

    WindowRefreshEvent& operator=(const WindowRefreshEvent& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const WindowRefreshEvent& rhs) {
      stream << "WindowRefreshEvent(";
      stream << "window=" << rhs.window() << ", ";
      stream << ")";
      return stream;
    }
  };

  class WindowMaximizeEvent : public WindowEvent {
  private:
    bool maximized_;
  public:
    WindowMaximizeEvent(Window* window, const bool maximized):
      WindowEvent(window),
      maximized_(maximized) {
    }
    WindowMaximizeEvent(const WindowMaximizeEvent& rhs) = default;
    ~WindowMaximizeEvent() override = default;

    bool maximized() const {
      return maximized_;
    }

    WindowMaximizeEvent& operator=(const WindowMaximizeEvent& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const WindowMaximizeEvent& rhs) {
      stream << "WindowMaximizedEvent(";
      stream << "window=" << rhs.window() << ", ";
      stream << "maximized=" << rhs.maximized();
      stream << ")";
      return stream;
    }
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
    WindowContentScaleEvent(const WindowContentScaleEvent& rhs) = default;
    ~WindowContentScaleEvent() override = default;

    glm::vec2 scale() const {
      return scale_;
    }

    WindowContentScaleEvent& operator=(const WindowContentScaleEvent& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const WindowContentScaleEvent& rhs) {
      stream << "WindowContentScaleEvent(";
      stream << "window=" << rhs.window() << ", ";
      stream << "scale=" << glm::to_string(rhs.scale());
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_WINDOW_EVENT_H