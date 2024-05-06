#include "mcc/window/window.h"

#include "mcc/os_thread.h"
#include "mcc/engine/engine.h"
#include "mcc/render/renderer.h"
#include "mcc/window/window_flags.h"

namespace mcc {
  namespace window {
    static rx::subject<WindowEvent*> events_;

    rx::observable<WindowEvent*> OnWindowEvent() {
      return events_.get_observable();
    }
  }

  void Window::Publish(WindowEvent* event) {
    using window::events_;
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    subscriber.on_next(event);
  }

  Window::Window(WindowHandle* handle):
    handle_(handle),
    sub_post_render_(),
    focused_(false) {
    sub_post_render_ = render::OnRenderEvent()
      .filter(render::PostRenderEvent::Filter)
      .map(render::PostRenderEvent::Cast)
      .subscribe([this](render::PostRenderEvent* event) {
        SwapBuffers();
      });
  }

  static constexpr const auto kInitialMonitorResolutionScale = 0.75;
  static inline int32_t
  GetInitialWidth(const Resolution& monitor_resolution) {
    switch(FLAGS_window_width) {
      case 0:
        return static_cast<int32_t>(monitor_resolution.width() * kInitialMonitorResolutionScale);
      default:
        return FLAGS_window_width;
    }
  }

  static inline int32_t
  GetInitialHeight(const Resolution& monitor_resolution) {
    switch(FLAGS_window_height) {
      case 0:
        return static_cast<int32_t>(monitor_resolution.height() * kInitialMonitorResolutionScale);
      default:
        return FLAGS_window_height;
    }
  }

  WindowSize Window::GetInitialSize() {
    Monitor monitor;
    const auto resolution = monitor.GetResolution();
    return { GetInitialWidth(resolution), GetInitialHeight(resolution) };
  }

  bool Window::IsFullscreen() {
    return FLAGS_fullscreen;
  }

  void Window::Init() {
    const auto engine = engine::Engine::GetEngine();
    engine->OnPreInitEvent().subscribe([](engine::PreInitEvent* e) {
      return Window::OnPreInit();
    });
    engine->OnPostInitEvent().subscribe([](engine::PostInitEvent* e) {
      return Window::OnPostInit();
    });
    engine->OnTerminatingEvent().subscribe([](engine::TerminatingEvent* e) {
      return Window::OnTerminating();
    });
    engine->OnTerminatedEvent().subscribe([](engine::TerminatedEvent* e) {
      return Window::OnTerminated();
    });
  }

  void Window::OnPreInit() {
    DLOG(INFO) << "[" << GetCurrentThreadName() << "] pre-init....";
    const auto window = Window::Get();
    window::OnWindowClosedEvent()
      .subscribe([](WindowClosedEvent* event) {
        DLOG(INFO) << "shutting down engine on thread: " << GetCurrentThreadName();
        const auto engine = engine::Engine::GetEngine();
        engine->Shutdown();
      });
  }

  void Window::OnPostInit() {
    const auto window = Window::Get();
    window->Open();
  }

  void Window::OnTerminating() {
    const auto window = Window::Get();
    window->Close();
  }

  void Window::OnTerminated() {
    glfwTerminate();
  }
}