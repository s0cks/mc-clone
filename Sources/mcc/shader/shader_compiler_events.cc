#include "mcc/shader/shader_compiler_events.h"
#include <sstream>

namespace mcc::shader {
  std::string ShaderCompiledEvent::ToString() const {
    std::stringstream ss;
    ss << "ShaderCompiledEvent(";
    ss << "id=" << GetShaderId();
    ss << ")";
    return ss.str();
  }
}