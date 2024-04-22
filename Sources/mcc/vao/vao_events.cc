#include "mcc/vao/vao_events.h"

namespace mcc::vao {
  std::string VaoCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "VaoCreatedEvent(";
    ss << "id=" << GetVaoId();
    ss << ")";
    return ss.str();
  }

  std::string VaoDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "VaoDestroyedEvent(";
    ss << "id=" << GetVaoId();
    ss << ")";
    return ss.str();
  }
}