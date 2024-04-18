#include "mcc/component/component_events.h"
#include <sstream>

namespace mcc::component {
  std::string ComponentRegisteredEvent::ToString() const {
    std::stringstream ss;
    ss << "ComponentRegisteredEvent(";
    ss << ")";
    return ss.str();
  }
}