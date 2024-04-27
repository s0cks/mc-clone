#include "mcc/gui/gui_component.h"

#include "mcc/mouse/mouse.h"

namespace mcc::gui {
  ComponentBase::ComponentBase():
    Component(),
    events_(),
    pos_(),
    size_() {
  }
}