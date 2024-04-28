#ifndef MCC_GUI_ON_MOUSEEXIT_H
#define MCC_GUI_ON_MOUSEEXIT_H

#include "mcc/gui/gui_events.h"
#include "mcc/gui/gui_constants.h"
#include "mcc/gui/gui_component.h"
#include "mcc/mouse/mouse_events.h"

namespace mcc::gui {
  class OnMouseExitEvent {
  private:
    rx::subscription sub_mouse_move_;
    rx::subscription sub_mouse_exit_;
    bool entered_;
    Component* component_;

    void OnMouseMove(mouse::MouseMoveEvent* event);
  protected:
    explicit OnMouseExitEvent(Component* component);
    virtual void OnMouseExit(gui::MouseExitEvent* event) = 0;
  public:
    virtual ~OnMouseExitEvent() {
      if(sub_mouse_move_.is_subscribed())
        sub_mouse_exit_.unsubscribe();
      if(sub_mouse_exit_.is_subscribed())
        sub_mouse_exit_.unsubscribe();
    }
  };
}

#endif //MCC_GUI_ON_MOUSEEXIT_H