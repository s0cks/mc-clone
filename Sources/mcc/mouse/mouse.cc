#include "mcc/mouse/mouse.h"

#include <units.h>
#include "mcc/uv/utils.h"
#include "mcc/thread_local.h"
#include "mcc/window/window.h"
#include "mcc/engine/engine.h"

#include "mcc/properties/properties.h"

namespace mcc::mouse {
  static ThreadLocal<Mouse> mouse_;
  static rx::subject<MouseEvent*> events_;

  rx::observable<MouseEvent*> OnMouseEvent() {
    return events_.get_observable();
  }

  void Mouse::PublishEvent(MouseEvent* event) {
    MCC_ASSERT(event);
    DLOG(INFO) << "publishing: " << event->ToString();
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
    PropertySet props;
    LOG_IF(ERROR, !properties::Parse(uri::Uri("file:mouse.properties"), props)) << "failed to parse Mouse properties.";

    DLOG(INFO) << "Mouse Properties:";
    properties::PropertySetPrinter::Print(props);

    window::OnWindowOpenedEvent()
      .subscribe([](WindowOpenedEvent* event) {
        const auto engine = engine::Engine::GetEngine();
        SetMouse(CreateMouse(engine, event->window()));
      });
    window::OnWindowClosedEvent()
      .subscribe([](WindowClosedEvent* event) {
        //TODO: delete mouse
      });
  }
}