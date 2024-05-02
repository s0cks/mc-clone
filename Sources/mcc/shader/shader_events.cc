#include <sstream>

#include "mcc/shader/shader_events.h"
#include "mcc/shader/shader.h"

namespace mcc::shader {
  ShaderId ShaderEvent::GetShaderId() const {
    return GetShader()->GetId();
  }

  std::string ShaderCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "ShaderCreatedEvent(";
    ss << "id=" << GetShaderId();
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