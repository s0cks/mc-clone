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

  static inline uint64_t
  GetShaderInfoLogLength(const ShaderId id) {
    MCC_ASSERT(IsValidShaderId(id));
    GLint length = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    CHECK_GL(FATAL);
    return static_cast<uint64_t>(length);
  }

  static inline std::string
  GetShaderInfoLog(const ShaderId id) {
    auto size = GetShaderInfoLogLength(id);
    uint8_t data[size];
    GLsizei length;
    glGetShaderInfoLog(id, size, &length, (GLchar*) data);
    CHECK_GL(FATAL);
    return std::string((const char*) &data[0], length);
  }

  ShaderCompilerStatus::ShaderCompilerStatus(const ShaderId shader):
    id(shader),
    compiled(GetCompileStatus(shader)),
    message() {
  }

  std::string ShaderCompilerStatus::ToString() const {
    std::stringstream ss;
    ss << "ShaderCompilerStatus(";
    ss << "id=" << id << ", ";
    ss << "compiled=" << compiled << ", ";
    ss << "message=" << (HasMessage() ? message : "None");
    ss << ")";
    return ss.str();
  }
}