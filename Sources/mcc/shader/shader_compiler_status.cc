#include "mcc/shader/shader_compiler_status.h"
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

  ShaderCompilerStatus::ShaderCompilerStatus(const ShaderId shader):
    id(shader),
    compiled(GetCompileStatus(shader)),
    info(shader) {
  }

  std::string ShaderCompilerStatus::ToString() const {
    std::stringstream ss;
    ss << "ShaderCompilerStatus(";
    ss << "id=" << id << ", ";
    ss << "compiled=" << compiled << ", ";
    ss << "message=" << (HasMessage() ? (const std::string&)info : "None");
    ss << ")";
    return ss.str();
  }
}