#ifndef MCC_WINDOW_H
#define MCC_WINDOW_H

#include "mcc/rx.h"
#include "mcc/window/monitor.h"
#include "mcc/window/window_event.h"
#include "mcc/window/window_glfw.h"

namespace mcc {
  typedef glm::u32vec2 WindowSize;

  class Window {
    friend class GlfwWindow;
  public:
    static WindowHandle* NewHandle(const WindowSize& size, const std::string& title);
    
    static void CloseWindow(WindowHandle* handle);
    static void SetWindowPos(WindowHandle* handle, const WindowPos& pos);
    static void SetWindowSize(WindowHandle* handle, const WindowSize& size);
    
    static void SetWindowTitle(WindowHandle* handle, const char* title);
    
    static inline void
    SetWindowTitle(WindowHandle* handle, const std::string& title) {
      return SetWindowTitle(handle, title.data());
    }

    static WindowSize GetWindowSize(WindowHandle* handle);
    static WindowPos GetWindowPos(WindowHandle* handle);
  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
    static void OnTerminating();
    static void OnTerminated();

    static void Set(Window* window);
  protected:
    WindowHandle* handle_;
    rxsub::subject<WindowEvent*> events_all_;
#define DEFINE_WINDOW_EVENT_SUBJECT(Name, Subject) \
    rxsub::subject<Name*> events_##Subject##_;
    FOR_EACH_WINDOW_EVENT(DEFINE_WINDOW_EVENT_SUBJECT)
#undef DEFINE_WINDOW_EVENT_SUBJECT

    explicit Window(WindowHandle* handle):
      handle_(handle),
      events_all_(),
      events_opened_(),
      events_closed_(),
      events_size_(),
      events_focus_() {
    }

    inline void SetHandle(WindowHandle* handle) {
      MCC_ASSERT(!HasHandle());
      handle_ = handle;
    }
  public:
    virtual ~Window() = default;

    inline WindowHandle* handle() const {
      return handle_;
    }

    inline bool HasHandle() const {
      return handle() != nullptr;
    }

    virtual void Close() = 0;

    inline WindowSize GetSize() const {
      return GetWindowSize(handle());
    }

    inline uint32_t GetWidth() const {
      const auto size = GetSize();
      return size[0];
    }

    inline uint32_t GetHeight() const {
      const auto size = GetSize();
      return size[1];
    }

    inline WindowPos GetPos() const {
      return GetWindowPos(handle());
    }

    rx::observable<WindowEvent*> on_event() const {
      return events_all_.get_observable();
    }

#define DEFINE_GET_WINDOW_EVENT_OBSERVABLE(Name, Subject) \
    rx::observable<Name*> on_##Subject() const { return events_##Subject##_.get_observable(); }
    FOR_EACH_WINDOW_EVENT(DEFINE_GET_WINDOW_EVENT_OBSERVABLE)
#undef DEFINE_GET_WINDOW_EVENT_OBSERVABLE

#define DEFINE_PUBLISH_WINDOW_EVENT(Name, Subject)      \
    inline void Publish(Name* next) { return events_##Subject##_.get_subscriber().on_next(next); }
    FOR_EACH_WINDOW_EVENT(DEFINE_PUBLISH_WINDOW_EVENT)
#undef DEFINE_PUBLISH_WINDOW_EVENT

    inline rx::observable<WindowFocusEvent*> on_focused() const {
      return on_focus().filter([](WindowFocusEvent* event) {
        return event->focused();
      });
    }

    inline rx::observable<WindowFocusEvent*> on_unfocused() const {
      return on_focus().filter([](WindowFocusEvent* event) {
        return !event->focused();
      });
    }

    inline float GetAspectRatio() {
      const auto size = GetSize();
      return size[0] / size[1];
    }

    inline glm::vec2 GetCenterCoord() {
      const auto size = GetSize();
      return glm::vec2(size[0] / 2, size[1] / 2);
    }

    friend std::ostream& operator<<(std::ostream& stream, const Window& rhs) {
      stream << "Window(";
      stream << "size=" << glm::to_string(rhs.GetSize()) << ", ";
      stream << "pos=" << glm::to_string(rhs.GetPos());
      stream << ")";
      return stream;
    }
  public:
    static void Init();
    static bool IsFullscreen();
    static WindowSize GetInitialSize();
    static Window* Get();
    static Window* New(const WindowSize& size, const std::string& title, Monitor* monitor = nullptr);

    static inline Window*
    New(const std::string& title) {
      return New(GetInitialSize(), title);
    }

    static inline Window*
    New() {
      return New("Hello World");
    }
  };
}

#include "mcc/window/window_glfw.h"

#endif //MCC_WINDOW_H