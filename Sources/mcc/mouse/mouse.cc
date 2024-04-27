#include "mcc/mouse/mouse.h"

#include <units.h>
#include "mcc/uv/utils.h"
#include "mcc/thread_local.h"
#include "mcc/window/window.h"
#include "mcc/engine/engine.h"

namespace mcc::mouse {
  static ThreadLocal<Mouse> mouse_;
  static rx::subject<MouseEvent*> events_;

  rx::observable<MouseEvent*> OnMouseEvent() {
    return events_.get_observable();
  }

  void Mouse::PublishEvent(MouseEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    subscriber.on_next(event);
  }

  static inline Mouse*
  CreateMouse(engine::Engine* engine, Window* window) {
    MCC_ASSERT(engine);
    MCC_ASSERT(window);
    return new GlfwMouse(engine, window);
  }

  static inline Mouse*
  SetMouse(Mouse* mouse) {
    MCC_ASSERT(mouse);
    mouse_.Set(mouse);
    return mouse;
  }

  Mouse* GetMouse() {
    MCC_ASSERT(mouse_);
    return mouse_.Get();
  }

  void InitMouse() {
    const auto window = Window::Get();
    if(!window) {
      LOG(ERROR) << "no window available to create mouse.";
      return;
    }

    window->OnOpened()
      .subscribe([](WindowEvent* event) {
        const auto engine = engine::Engine::GetEngine();
        SetMouse(CreateMouse(engine, event->window()));
      });
    window->OnClosed()
      .subscribe([](WindowEvent* event) {
        //TODO: delete mouse
      });
  }
}