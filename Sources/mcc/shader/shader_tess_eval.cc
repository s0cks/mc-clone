#include "mcc/shader/shader.h"

namespace mcc::shader {
  const std::set<std::string> TessEvalShader::kValidExtensions = {
    "json",
    "glsl",
    "tese",
  };
  
  std::string TessEvalShader::ToString() const {
    std::stringstream ss;
    ss << "TessEvalShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }
}