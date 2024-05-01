#include "mcc/material/material_events.h"
#include <sstream>

namespace mcc::material {
  std::string MaterialCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "MaterialCreatedEvent(";
    ss << ")";
    return ss.str();
  }

  std::string MaterialDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "MaterialDestroyedEvent(";
    ss << ")";
    return ss.str();
  }
}