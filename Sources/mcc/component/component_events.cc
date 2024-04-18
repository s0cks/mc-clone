#include "mcc/component/component_events.h"

#include <sstream>
#include "mcc/component/component.h"

namespace mcc::component {
  std::string ComponentRegisteredEvent::ToString() const {
    std::stringstream ss;
    ss << "ComponentRegisteredEvent(";
    ss << "component=" << component()->GetName();
    ss << ")";
    return ss.str();
  }

  std::string ComponentStateCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "ComponentStateCreatedEvent(";
    ss << "component=" << component()->GetName();
    ss << ")";
    return ss.str();
  }

  std::string ComponentStateDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "ComponentStateDestroyedEvent(";
    ss << "component=" << component()->GetName();
    ss << ")";
    return ss.str();
  }
}