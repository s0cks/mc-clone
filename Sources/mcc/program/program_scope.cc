#include "mcc/program/program_scope.h"
#include "mcc/program/program.h"

namespace mcc::program {
  ProgramId ProgramScope::GetProgramId() const {
    return GetProgram()->GetProgramId();
  }

  void ProgramUboBindScope::Bind(const std::string& name, Ubo* ubo) {
    MCC_ASSERT(ubo);
    const auto index = index_++;
    const auto bound = BoundUbo {
      .index = index,
      .name = name,
      .ubo = ubo,
    };
    const auto inserted = bound_.insert(bound);
    if(!inserted.second) {
      LOG(ERROR) << "failed to insert: " << bound;
      return;
    }
    glBindBufferRange(GL_UNIFORM_BUFFER, index, ubo->GetId(), 0, ubo->GetTotalSize());
    CHECK_GL(FATAL);
    GetProgram()->SetUniformBlock(name, index);
  }
}