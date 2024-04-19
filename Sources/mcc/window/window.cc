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
    window->OnClosed().subscribe([&window](WindowClosedEvent* event) {
      DLOG(INFO) << "shutting down engine on thread: " << GetCurrentThreadName();
      const auto engine = engine::Engine::GetEngine();
      engine->Shutdown();
    });
    window->OnFocused().subscribe([](WindowFocusEvent* event) {
      DLOG(INFO) << "window focused.";
    });
    window->OnUnfocused().subscribe([](WindowFocusEvent* event) {
      DLOG(INFO) << "window unfocused.";
    });
    window->OnOpened().subscribe([](WindowOpenedEvent* event) {
      DLOG(INFO) << "window opened.";
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