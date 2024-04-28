#ifndef MCC_GUI_ON_MOUSECLICK_H
#define MCC_GUI_ON_MOUSECLICK_H

#include "mcc/gui/gui_events.h"
#include "mcc/gui/gui_component.h"
#include "mcc/mouse/mouse_events.h"

namespace mcc::gui {
  class OnMouseClickEvent {
  private:
    rx::subscription sub_mouse_pressed_;
    rx::subscription sub_mouse_click_;
    Component* component_;
    void OnMouseButtonPressed(mouse::MouseButtonPressedEvent* event);
  protected:
    explicit OnMouseClickEvent(Component* component);
    virtual void OnMouseClick(gui::MouseClickEvent* event) = 0;
  public:
    virtual ~OnMouseClickEvent() {
      if(sub_mouse_pressed_.is_subscribed())
        sub_mouse_pressed_.unsubscribe();
      if(sub_mouse_click_.is_subscribed())
        sub_mouse_click_.unsubscribe();
    }
  };
}

#endif //MCC_GUI_ON_MOUSECLICK_H