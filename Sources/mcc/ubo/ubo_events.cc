#include "mcc/ubo/ubo_events.h"

#include <sstream>
#include "mcc/ubo/ubo.h"

namespace mcc::ubo {
  UboId UboEvent::GetUboId() const {
    return GetUbo()->GetId();
  }

  std::string UboCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "UboCreatedEvent(";
    ss << "id=" << GetUboId();
    ss << ")";
    return ss.str();
  }

  std::string UboDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "UboDestroyedEvent(";
    ss << "id=" << GetUboId();
    ss << ")";
    return ss.str();
  }
}