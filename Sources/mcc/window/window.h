#ifndef MCC_WINDOW_H
#define MCC_WINDOW_H

#include "mcc/window/monitor.h"
#include "mcc/window/window_glfw.h"
#include "mcc/window/window_events.h"

namespace mcc {
  typedef glm::u32vec2 WindowSize;

  namespace window {
    rx::observable<WindowEvent*> OnWindowEvent();

#define DEFINE_ON_WINDOW_EVENT(Name)                            \
    static inline rx::observable<Name##Event*>                  \
    On##Name##Event() {                                         \
      return OnWindowEvent()                                    \
        .filter(Name##Event::Filter)                            \
        .map(Name##Event::Cast);                                \
    }
    FOR_EACH_WINDOW_EVENT(DEFINE_ON_WINDOW_EVENT)
#undef DEFINE_ON_WINDOW_EVENT
  }

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
    static WindowSize GetWindowFramebufferSize(WindowHandle* handle);
    static WindowPos GetWindowPos(WindowHandle* handle);
  private:
    static void OnPreInit();
    static void OnPostInit();
    static void OnTerminating();
    static void OnTerminated();

    static void Set(Window* window);
  protected:
    WindowHandle* handle_;
    rx::subscription sub_post_render_;

    explicit Window(WindowHandle* handle);

    inline void SetHandle(WindowHandle* handle) {
      MCC_ASSERT(!HasHandle());
      handle_ = handle;
    }

    static void Publish(WindowEvent* event);

    template<class E, typename... Args>
    static inline void
    Publish(Args... args) {
      E event(args...);
      return Publish((WindowEvent*) &event);
    }
  public:
    virtual ~Window() {
      if(sub_post_render_.is_subscribed())
        sub_post_render_.unsubscribe();
    }

    inline WindowHandle* handle() const {
      return handle_;
    }

    inline bool HasHandle() const {
      return handle() != nullptr;
    }

    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual void SwapBuffers() = 0;

    inline WindowSize GetSize() const {
      return GetWindowSize(handle());
    }

    inline WindowSize GetFramebufferSize() const {
      return GetWindowFramebufferSize(handle());
    }

    inline float GetFramebufferAspectRatio() const {
      const auto fb_size = GetFramebufferSize();
      return fb_size[0] / fb_size[1];
    }

    inline glm::vec2 GetFramebufferScale() const {
      const auto size = GetSize();
      const auto fb_size = GetFramebufferSize();
      return {
        fb_size[0] / size[0],
        fb_size[1] / size[1],
      };
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

    inline float GetAspectRatio() {
      const auto size = GetSize();
      return size[0] / size[1];
    }

    inline glm::vec2 GetCenterCoord() {
      const auto size = GetSize();
      return glm::vec2(size[0] / 2, size[1] / 2);
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