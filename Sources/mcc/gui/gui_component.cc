#include "mcc/gui/gui_component.h"

#include "mcc/mouse/mouse.h"

namespace mcc::gui {
  ComponentBase::ComponentBase():
    Component(),
    events_(),
    pos_(),
    size_(),
    on_mouse_event_() {
    const auto mouse = mouse::GetMouse();
    MCC_ASSERT(mouse);
    on_mouse_event_ = mouse->OnEvent()
      .subscribe([this](mouse::MouseEvent* event) {
        return OnMouseEvent(event);
      });
  }

  void ComponentBase::OnMouseEvent(mouse::MouseEvent* event) {
    if(event->IsMouseMoveEvent()) {
      const auto& pos = event->AsMouseMoveEvent()->pos();
      Point point(pos[0], pos[1]);
      if(Contains(point)) {
        entered_ = true;
        Publish<MouseEnterEvent>(point);
      } else if(!(entered_ && Contains(point))) {
        entered_ = false;
        Publish<MouseExitEvent>(point);
      }
    }
  }
}