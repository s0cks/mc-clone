#include "mcc/window/window.h"

#include "mcc/gfx.h"
#include "mcc/thread_local.h"

#include "mcc/window/monitor.h"

namespace mcc {
  class GlfwWindow : public Window {
  public:
    explicit GlfwWindow(WindowHandle* handle):
      Window(handle) {
      glfwSetWindowUserPointer(handle, this);
    }
    ~GlfwWindow() override = default;

    void Close() override {
      CloseWindow(handle());
    }
  };

  static inline GlfwWindow*
  GetGlfwWindow(WindowHandle* handle) {
    return (GlfwWindow*) glfwGetWindowUserPointer(handle);
  }

  template<typename E, typename... Args>
  static inline void
  PublishEvent(WindowHandle* handle, Args... args) {
    const auto win = GetGlfwWindow(handle);
    E event(win, args...);
    return win->Publish(&event);
  }

  static inline void
  OnWindowClosed(WindowHandle* handle) {
    return PublishEvent<WindowClosedEvent>(handle);
  }

  static inline void
  OnWindowPos(WindowHandle* handle, const int xPos, const int yPos) {
    return PublishEvent<WindowPosEvent>(handle, xPos, yPos);
  }

  static inline void
  OnWindowSize(WindowHandle* handle, const int width, const int height) {
    return PublishEvent<WindowSizeEvent>(handle, width, height);
  }

  static inline void
  OnWindowFocus(WindowHandle* handle, int focused) {
    return PublishEvent<WindowFocusEvent>(handle, static_cast<bool>(focused));
  }

  static inline void
  OnWindowIconify(WindowHandle* handle, int iconified) {
    return PublishEvent<WindowIconifyEvent>(handle, static_cast<bool>(iconified));
  }

  static inline void
  OnWindowRefresh(WindowHandle* handle) {
    return PublishEvent<WindowRefreshEvent>(handle);
  }
  
  static inline void
  OnWindowMaximize(WindowHandle* handle, int maximized) {
    return PublishEvent<WindowMaximizeEvent>(handle, static_cast<bool>(maximized));
  }

  static inline void
  OnWindowContentScale(WindowHandle* handle, float xScale, float yScale) {
    return PublishEvent<WindowContentScaleEvent>(handle, xScale, yScale);
  }

  WindowHandle* NewWindowHandle(const WindowSize size,
                                const std::string& title,
                                Monitor* monitor,
                                WindowHandle* share = nullptr) {
    const auto handle = glfwCreateWindow(size[0], size[1], title.data(), monitor ? monitor->handle() : nullptr, share);
    if(!handle) {
      glfwTerminate();
      LOG(FATAL) << "failed to create GlfwWindow handle.";
      return nullptr;
    }

    glfwMakeContextCurrent(handle);
    glfwSetWindowTitle(handle, title.data());
    glfwSetWindowSize(handle, size[0], size[1]);
    if(!monitor) {
      Monitor m;
      const auto resolution = m.GetResolution();
      const auto centered = glm::u32vec2((resolution[0] - size[0]) / 2, (resolution[1] - size[1]) / 2);
      glfwSetWindowPos(handle, centered[0], centered[1]);
    }
    
    glfwSetWindowCloseCallback(handle, &OnWindowClosed);
    glfwSetWindowPosCallback(handle, &OnWindowPos);
    glfwSetWindowSizeCallback(handle, &OnWindowSize);
    glfwSetWindowFocusCallback(handle, &OnWindowFocus);
    glfwSetWindowIconifyCallback(handle, &OnWindowIconify);
    glfwSetWindowRefreshCallback(handle, &OnWindowRefresh);
    glfwSetWindowMaximizeCallback(handle, &OnWindowMaximize);
    glfwSetWindowContentScaleCallback(handle, &OnWindowContentScale);
    glfwSwapInterval(0);
    return handle;
  }

  WindowHandle* Window::NewHandle(const WindowSize& size,
                                  const std::string& title) {
    Monitor monitor;
    return NewWindowHandle(size, title, &monitor);
  }

  void Window::CloseWindow(WindowHandle* handle) {
    DLOG(INFO) << "closing window....";
    glfwSetWindowShouldClose(handle, GL_TRUE);
  }

  void Window::SetWindowSize(WindowHandle* handle, const WindowSize& size) {
    glfwSetWindowSize(handle, size[0], size[1]);
  }

  WindowSize Window::GetWindowSize(WindowHandle* handle) {
    WindowSize size;
    glfwGetWindowSize(handle, (int*) &size[0], (int*) &size[1]);
    return size;
  }

  void Window::SetWindowPos(WindowHandle* handle, const WindowPos& pos) {
    glfwSetWindowPos(handle, pos[0], pos[1]);
  }

  WindowPos Window::GetWindowPos(WindowHandle* handle) {
    WindowPos pos;
    glfwGetWindowPos(handle, (int*) &pos[0], (int*) &pos[1]);
    return pos;
  }

  void Window::SetWindowTitle(WindowHandle* handle, const char* title) {
    glfwSetWindowTitle(handle, title);
  }

  static ThreadLocal<Window> window_;

  Window* Window::New(const WindowSize& size, const std::string& title, Monitor* monitor) {
    return new GlfwWindow(NewWindowHandle(size, title, monitor));
  }

  void Window::Set(Window* window) {
    window_.Set(window);
  }

  Window* Window::Get() {
    auto window = window_.Get();
    if(!window)
      window_.Set(window = Window::New());
    return window;
  }
}