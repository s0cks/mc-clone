#include "mcc/vao/vao_events.h"
#include "mcc/vao/vao.h"

namespace mcc::vao {
  VaoId VaoEvent::GetVaoId() const {
    return GetVao()->GetId();
  }

  std::string VaoCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "VaoCreatedEvent(";
    ss << "vao=" << GetVao()->ToString();
    ss << ")";
    return ss.str();
  }

  std::string VaoDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "VaoDestroyedEvent(";
    ss << "vao=" << GetVao()->ToString();
    ss << ")";
    return ss.str();
  }
}