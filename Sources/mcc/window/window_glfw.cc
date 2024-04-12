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

    template<class E, typename... Args>
    inline void Publish(Args... args) {
      E event(this, args...);
      DLOG(INFO) << "publishing " << event << "....";
      events_.get_subscriber().on_next(&event);
    }

    void Open() override {
      Publish<WindowOpenedEvent>();
    }

    void Close() override {
      CloseWindow(handle());
    }
  };

  static inline GlfwWindow*
  GetGlfwWindow(WindowHandle* handle) {
    return (GlfwWindow*) glfwGetWindowUserPointer(handle);
  }

  static inline void
  OnWindowClosed(WindowHandle* handle) {
    return GetGlfwWindow(handle)->Publish<WindowClosedEvent>();
  }

  static inline void
  OnWindowPos(WindowHandle* handle, const int xPos, const int yPos) {
    return GetGlfwWindow(handle)->Publish<WindowPosEvent>(xPos, yPos);
  }

  static inline void
  OnWindowSize(WindowHandle* handle, const int width, const int height) {
    return GetGlfwWindow(handle)->Publish<WindowSizeEvent>(width, height);
  }

  static inline void
  OnWindowFocus(WindowHandle* handle, int focused) {
    return GetGlfwWindow(handle)->Publish<WindowFocusEvent>(static_cast<bool>(focused));
  }

  static inline void
  OnWindowIconify(WindowHandle* handle, int iconified) {
    return GetGlfwWindow(handle)->Publish<WindowIconifyEvent>(static_cast<bool>(iconified));
  }

  static inline void
  OnWindowRefresh(WindowHandle* handle) {
    return GetGlfwWindow(handle)->Publish<WindowRefreshEvent>();
  }
  
  static inline void
  OnWindowMaximize(WindowHandle* handle, int maximized) {
    return GetGlfwWindow(handle)->Publish<WindowMaximizeEvent>(static_cast<bool>(maximized));
  }

  static inline void
  OnWindowContentScale(WindowHandle* handle, float xScale, float yScale) {
    return GetGlfwWindow(handle)->Publish<WindowContentScaleEvent>(xScale, yScale);
  }

  WindowHandle* NewWindowHandle(const WindowSize size,
                                const std::string& title,
                                Monitor* monitor,
                                WindowHandle* share = nullptr) {
    DLOG(INFO) << "creating glfw " << title << " window with size " << glm::to_string(size) << "....";
    const auto handle = glfwCreateWindow(size[0], size[1], title.data(), monitor ? monitor->handle() : nullptr, share);
    if(!handle) {
      glfwTerminate();
      LOG(FATAL) << "failed to create GlfwWindow handle.";
      return nullptr;
    }

    glfwSetWindowTitle(handle, title.data());
    glfwSetWindowSize(handle, size[0], size[1]);
    if(!monitor) {
      // Monitor m;
      // DLOG(INFO) << "monitor: " << m;
      // const auto resolution = m.GetResolution();
      // const auto centered = glm::u32vec2((resolution[0] - size[0]) / 2, (resolution[1] - size[1]) / 2);
      // DLOG(INFO) << "centered: " << glm::to_string(centered);
      // glfwSetWindowPos(handle, centered[0], centered[1]);
    }
    
    glfwSetWindowCloseCallback(handle, &OnWindowClosed);
    glfwSetWindowPosCallback(handle, &OnWindowPos);
    glfwSetWindowSizeCallback(handle, &OnWindowSize);
    glfwSetWindowFocusCallback(handle, &OnWindowFocus);
    glfwSetWindowIconifyCallback(handle, &OnWindowIconify);
    glfwSetWindowRefreshCallback(handle, &OnWindowRefresh);
    glfwSetWindowMaximizeCallback(handle, &OnWindowMaximize);
    glfwSetWindowContentScaleCallback(handle, &OnWindowContentScale);
    glfwMakeContextCurrent(handle);
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
    int size[2];
    glfwGetWindowSize(handle, (int*) &size[0], (int*) &size[1]);
    return { size[0], size[1] };
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