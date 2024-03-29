#include "mcc/window/window.h"

#include "mcc/flags.h"
#include "mcc/engine/engine.h"

#include "mcc/os_thread.h"

namespace mcc {
  WindowSize Window::GetInitialSize() {
    return { FLAGS_width, FLAGS_height };
  }

  bool Window::IsFullscreen() {
    return FLAGS_fullscreen;
  }

  void Window::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    Engine::OnTerminating(&OnTerminating);
    Engine::OnTerminated(&OnTerminated);
  }

  void Window::OnPreInit() {
    const auto window = Window::Get();
    window->on_closed().subscribe([&window](WindowClosedEvent* event) {
      DLOG(INFO) << "shutting down engine on thread: " << GetCurrentThreadName();
      Engine::Shutdown();
    });
    window->on_focused().subscribe([](WindowFocusEvent* event) {
      DLOG(INFO) << "window focused.";
    });
    window->on_unfocused().subscribe([](WindowFocusEvent* event) {
      DLOG(INFO) << "window unfocused.";
    });
  }

  void Window::OnInit() {
    
  }

  void Window::OnPostInit() {

  }

  void Window::OnTerminating() {
    const auto window = Window::Get();
    window->Close();
  }

  void Window::OnTerminated() {
    glfwTerminate();
  }
}