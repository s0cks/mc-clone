#ifndef MCC_GUI_ON_MOUSEENTER_H
#define MCC_GUI_ON_MOUSEENTER_H

#include "mcc/gui/gui_events.h"
#include "mcc/gui/gui_constants.h"
#include "mcc/gui/gui_component.h"
#include "mcc/mouse/mouse_events.h"

namespace mcc::gui {
  class OnMouseEnterEvent {
  private:
    rx::subscription sub_;
    bool entered_;
    Component* component_;

    void OnMouseMove(mouse::MouseMoveEvent* event);
  protected:
    explicit OnMouseEnterEvent(Component* component);
    virtual void OnMouseEnter(gui::MouseEnterEvent* event) = 0;
  public:
    virtual ~OnMouseEnterEvent() {
      if(sub_.is_subscribed())
        sub_.unsubscribe();
    }
  };
}

#endif //MCC_GUI_ON_MOUSEENTER_H