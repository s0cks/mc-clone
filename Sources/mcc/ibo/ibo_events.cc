#include "mcc/ibo/ibo_events.h"

#include <sstream>
#include "mcc/ibo/ibo.h"

namespace mcc::ibo {
  IboId IboEvent::GetIboId() const {
    return GetIbo()->GetId();
  }

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