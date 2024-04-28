#include "mcc/gui/gui_on_mouseclick.h"
#include "mcc/mouse/mouse.h"

namespace mcc::gui {
  OnMouseClickEvent::OnMouseClickEvent(Component* component):
    sub_mouse_click_(),
    sub_mouse_pressed_(),
    component_(component) {
    sub_mouse_pressed_ = mouse::OnMouseButtonPressedEvent()
      .filter([this](mouse::MouseButtonPressedEvent* event) {
        return event
            && component_->Contains(event->GetPos());
      })
      .subscribe([this](mouse::MouseButtonPressedEvent* event) {
        return OnMouseButtonPressed(event);
      });
    sub_mouse_click_ = component_->OnEvent()
      .filter(MouseClickEvent::Filter)
      .map(MouseClickEvent::Cast)
      .subscribe([this](MouseClickEvent* event) {
        return OnMouseClick(event);
      });
  }

  void OnMouseClickEvent::OnMouseButtonPressed(mouse::MouseButtonPressedEvent* event) {
    component_->Publish<MouseClickEvent>(Point(), event->GetButton());
  }
}