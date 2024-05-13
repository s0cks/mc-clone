#include "mcc/shader/shader.h"

namespace mcc::shader {
  const std::set<std::string> GeometryShader::kValidExtensions = {
    "json",
    "glsl",
    "geom",
  };

  std::string GeometryShader::ToString() const {
    std::stringstream ss;
    ss << "GeometryShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }
}