#include "mcc/shader/shader_linker.h"
#include "mcc/shader/shader_status.h"

namespace mcc::shader {
  ShaderId ShaderLinker::LinkShader(const ShaderId dst, const ShaderId src) {
    DLOG(INFO) << "attaching  " << src << " to " << dst;
    glAttachShader(dst, src);
    CHECK_GL(FATAL);
    return dst;
  }

  ShaderId ShaderLinker::LinkProgram(const ShaderId id) {
    glLinkProgram(id);
    CHECK_GL(FATAL);
    const auto status = ShaderLinkStatus(id);
    return !status ? kInvalidShaderId : id;
  }
}