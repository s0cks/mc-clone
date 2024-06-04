#include "mcc/gui/gui_on_mouseenter.h"

#include "mcc/mouse/mouse.h"
#include "mcc/gui/gui_component.h"

namespace mcc::gui {
  OnMouseEnterEvent::OnMouseEnterEvent(Component* component):
    sub_mouse_move_(),
    sub_mouse_enter_(),
    component_(component) {
    MCC_ASSERT(component);
    sub_mouse_move_ = mouse::OnMouseMoveEvent()
      .subscribe([this](mouse::MouseMoveEvent* event) {
        return OnMouseMove(event);
      });
    sub_mouse_enter_ = component_->OnMouseEnter()
      .subscribe([this,component](MouseEnterEvent* event) {
        return OnMouseEnter(event);
      });
  }

  void OnMouseEnterEvent::OnMouseMove(mouse::MouseMoveEvent* event) {
    const auto& pos = event->pos();
    auto& flags = component_->GetFlags();
    if(!flags.IsEntered() && component_->Contains(pos)) {
      flags.SetEntered();
      component_->Publish<MouseEnterEvent>(pos);
    }
  }
}