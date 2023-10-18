#include "mcc/engine.h"
#include "mcc/window.h"
#include "mcc/os_thread.h"
#include "mcc/thread_local.h"
#include "mcc/camera/camera.h"

namespace mcc {
  static ThreadLocal<Window> window_;

  void Engine::SetWindow(Window* value) {
    window_.Set(value);
  }

  Window* Engine::GetWindow() {
    return window_.Get();
  }
}