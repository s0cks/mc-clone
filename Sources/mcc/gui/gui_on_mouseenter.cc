#include "mcc/gui/gui_on_mouseenter.h"

#include "mcc/mouse/mouse.h"
#include "mcc/gui/gui_component.h"

namespace mcc::gui {
  OnMouseEnterEvent::OnMouseEnterEvent(Component* component):
    sub_mouse_move_(),
    sub_mouse_enter_(),
    entered_(false),
    component_(component) {
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
    if(!entered_ && component_->Contains(pos)) {
      entered_ = true;
      component_->Publish<MouseEnterEvent>(pos);
    } else if(entered_ && !component_->Contains(pos)) {
      entered_ = false;
    }
  }
}