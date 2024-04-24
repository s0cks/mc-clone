#include "mcc/rbo/rbo_events.h"

#include <sstream>
#include "mcc/rbo/rbo.h"

namespace mcc::rbo {
  RboId RboEvent::GetRboId() const {
    return GetRbo()->GetId();
  }

  std::string RboCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "RboCreatedEvent(";
    ss << "id=" << GetRboId();
    ss << ")";
    return ss.str();
  }
}