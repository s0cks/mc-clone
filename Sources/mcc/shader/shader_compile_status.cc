#include "mcc/shader/shader_compile_status.h"
#include <sstream>

namespace mcc::shader {
  static inline bool
  GetCompileStatus(const ShaderId id) {
    MCC_ASSERT(IsValidShaderId(id));
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    CHECK_GL(FATAL);
    return status == GL_TRUE;
  }

  ShaderCompileStatus::ShaderCompileStatus(const ShaderId id):
    id_(id),
    compiled_(GetCompileStatus(id)),
    info_(id) {
  }

  std::string ShaderCompileStatus::ToString() const {
    std::stringstream ss;
    ss << "ShaderCompileStatus(";
    ss << "id=" << GetShaderId() << ", ";
    ss << "compiled=" << (IsCompiled() ? 'y' : 'n') << ", ";
    ss << "message=" << (HasMessage() ? (const std::string&)GetInfo() : "None");
    ss << ")";
    return ss.str();
  }
}