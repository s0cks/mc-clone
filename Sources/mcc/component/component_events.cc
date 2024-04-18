#include "mcc/component/component_events.h"
#include <sstream>

namespace mcc::component {
  std::string ComponentRegisteredEvent::ToString() const {
    std::stringstream ss;
    ss << "ComponentRegisteredEvent(";
    ss << ")";
    return ss.str();
  }

  std::string ComponentCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "ComponentCreatedEvent(";
    ss << ")";
    return ss.str();
  }

  std::string ComponentDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "ComponentDestroyedEvent(";
    ss << ")";
    return ss.str();
  }
}