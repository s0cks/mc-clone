#include "mcc/shader/shader_linker.h"

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
    ProgramStatus status(GL_LINK_STATUS, id, kProgramShader);
    if(!status)
      throw std::runtime_error(fmt::format("failed to link program: {0:s}", status.message()));
    return id;
  }
}