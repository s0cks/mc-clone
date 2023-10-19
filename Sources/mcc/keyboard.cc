#include "mcc/window.h"
#include "mcc/keyboard.h"
#include "mcc/render_loop.h"

namespace mcc {
  bool IsKeyState(Window* window, const KeyCode key, const KeyState state) {
    return static_cast<KeyState>(glfwGetKey(window->handle(), static_cast<int>(key))) == state;
  }

  static inline uv::IdleCallback
  CreateCallback(const KeyCode key, const KeyState state, KeyStateCallback callback) {
    return [=]() {
      const auto window = Window::GetWindow();
      if(IsKeyState(window, key, state))
        callback();
    };
  }

  KeyStateListener::KeyStateListener(const KeyCode key, const KeyState state, KeyStateCallback callback):
    handle_(RenderLoop::GetRenderLoop()->GetLoop(), CreateCallback(key, state, std::move(callback))) {
  }
}