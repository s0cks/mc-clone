#include "mcc/ibo/ibo_events.h"

#include <sstream>

namespace mcc::ibo {
  std::string IboCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "IboCreatedEvent(";
    ss << "id=" << GetIboId();
    ss << ")";
    return ss.str();
  }

  std::string IboDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "IboDestroyedEvent(";
    ss << "id=" << GetIboId();
    ss << ")";
    return ss.str();
  }
}