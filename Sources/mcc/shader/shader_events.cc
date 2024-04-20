#include "mcc/shader/shader_events.h"
#include <sstream>

namespace mcc::shader {
  std::string ShaderCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "ShaderCreatedEvent(";
    ss << "id=" << GetShaderId();
    ss << ")";
    return ss.str();
  }

  std::string ShaderCompiledEvent::ToString() const {
    std::stringstream ss;
    ss << "ShaderCompiledEvent(";
    ss << ")";
    return ss.str();
  }

  std::string ShaderDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "ShaderDestroyedEvent(";
    ss << "id=" << GetShaderId();
    ss << ")";
    return ss.str();
  }
}