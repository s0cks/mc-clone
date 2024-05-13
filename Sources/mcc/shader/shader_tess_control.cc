#include "mcc/shader/shader.h"

namespace mcc::shader {
  const std::set<std::string> TessControlShader::kValidExtensions = {
    "json",
    "glsl",
    "tesc",
  };

  std::string TessControlShader::ToString() const {
    std::stringstream ss;
    ss << "TessControlShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }
}