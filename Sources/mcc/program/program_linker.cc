#include "mcc/program/program_linker.h"
#include <algorithm>

namespace mcc::program {
  void ProgramLinker::Attach(const ShaderId id) {
    MCC_ASSERT(shader::IsValidShaderId(id));
    MCC_ASSERT(!IsAttached(id));
    DLOG(INFO) << "attaching shader " << id << " to " << program() << ".";
    glAttachShader(program(), id);
    CHECK_GL(FATAL);
    attached_.insert(id);
  }

  void ProgramLinker::Detach(const ShaderId id, const bool remove) {
    MCC_ASSERT(shader::IsValidShaderId(id));
    MCC_ASSERT(IsAttached(id));
    DLOG(INFO) << "detattaching shader " << id << " from " << program() << ".";
    glDetachShader(program(), id);
    CHECK_GL(FATAL);
    if(remove)
      attached_.erase(id);
  }

  ProgramLinkStatus ProgramLinker::LinkProgram() {
    glLinkProgram(program());
    CHECK_GL(FATAL);

    const auto status = ProgramLinkStatus(program());
    if(status)
      DetachAll();
    return status;
  }
}