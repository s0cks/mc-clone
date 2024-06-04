#include "mcc/gui/gui_on_mouseexit.h"

#include "mcc/mouse/mouse.h"
#include "mcc/gui/gui_component.h"

namespace mcc::gui {
  OnMouseExitEvent::OnMouseExitEvent(Component* component):
    sub_mouse_move_(),
    sub_mouse_exit_(),
    component_(component) {
    MCC_ASSERT(component);
    sub_mouse_move_ = mouse::OnMouseMoveEvent()
      .subscribe([this,component](mouse::MouseMoveEvent* event) {
        return OnMouseMove(event);
      });
    sub_mouse_exit_ = component->OnMouseExit()
      .subscribe([this,component](MouseExitEvent* event) {
        return OnMouseExit(event);
      });
  }

  void OnMouseExitEvent::OnMouseMove(mouse::MouseMoveEvent* event) {
    const auto& pos = event->pos();
    auto& flags = component_->GetFlags();
    if(flags.IsEntered() && !component_->Contains(pos)) {
      flags.ClearEntered();
      component_->Publish<MouseExitEvent>(pos);
    }
  }
}