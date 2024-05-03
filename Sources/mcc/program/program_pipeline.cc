#include "mcc/program/program_pipeline.h"

namespace mcc::program {
  const ApplyProgramPipeline::SetUniformsCallback ApplyProgramPipeline::kDoNothing = [](const ProgramRef& ref) {
    // do nothing.
  };

  bool ApplyProgramPipeline::Apply() {
    GetProgramRef()->Apply();
    set_uniforms_(GetProgramRef());
    GetProgramRef()->Apply();
    return true;
  }
}