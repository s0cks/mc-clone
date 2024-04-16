#include "mcc/mouse/mouse_event.h"
#include <sstream>

namespace mcc::mouse {
  std::string MouseButtonEvent::ToString() const {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
  }

  std::string MouseMoveEvent::ToString() const {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
  }
}