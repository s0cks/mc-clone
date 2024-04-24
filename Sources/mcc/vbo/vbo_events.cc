#include "mcc/vbo/vbo_events.h"

#include <sstream>
#include "mcc/vbo/vbo.h"

namespace mcc::vbo {
  std::string VboCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "VboDestroyedEvent(";
    ss << "id=" << GetVboId();
    ss << ")";
    return ss.str();
  }

  std::string VboDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "VboDestroyedEvent(";
    ss << "id=" << GetVboId();
    ss << ")";
    return ss.str();
  }
}