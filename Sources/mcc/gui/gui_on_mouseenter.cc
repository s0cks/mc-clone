#include "mcc/gui/gui_on_mouseenter.h"

#include "mcc/mouse/mouse.h"
#include "mcc/gui/gui_component.h"

namespace mcc::gui {
  OnMouseEnterEvent::OnMouseEnterEvent(Component* component):
    sub_(),
    entered_(false),
    component_(component) {
    sub_ = mouse::OnMouseMoveEvent()
      .subscribe([this](mouse::MouseMoveEvent* event) {
        return OnMouseMove(event);
      });
  }

  void OnMouseEnterEvent::OnMouseMove(mouse::MouseMoveEvent* event) {
    DLOG(INFO) << "mouse move: " << event->ToString();
    const auto& pos = event->pos();
    DLOG(INFO) << "contains: " << component_->Contains(pos);
    if(component_->Contains(pos)) {
      DLOG(INFO) << "entered.";
      entered_ = true;
      component_->Publish<MouseEnterEvent>(pos);
    } else if(entered_ && !component_->Contains(pos)) {
      DLOG(INFO) << "exited.";
      entered_ = false;
    }
  }
}