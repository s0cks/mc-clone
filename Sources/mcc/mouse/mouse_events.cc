#include "mcc/mouse/mouse_events.h"
#include <sstream>

namespace mcc::mouse {
  std::string MouseCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "MouseCreatedEvent(";
    ss << ")";
    return ss.str();
  }

  std::string MouseDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "MouseDestroyedEvent(";
    ss << ")";
    return ss.str();
  }

  std::string MouseButtonPressedEvent::ToString() const {
    std::stringstream ss;
    ss << "MouseButtonPressedEvent(";
    ss << ")";
    return ss.str();
  }

  std::string MouseButtonReleasedEvent::ToString() const {
    std::stringstream ss;
    ss << "MouseButtonReleasedEvent(";
    ss << ")";
    return ss.str();
  }

  std::string MouseMoveEvent::ToString() const {
    std::stringstream ss;
    ss << "MouseMoveEvent(";
    ss << ")";
    return ss.str();
  }
}