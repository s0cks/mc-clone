#include "mcc/program/program_pipeline.h"

namespace mcc::program {
  const ApplyProgramPipeline::SetUniformsCallback ApplyProgramPipeline::kDoNothing = [](const ProgramRef& ref) {
    // do nothing.
  };

  void ApplyProgramPipeline::Render() {
    GetProgramRef()->Apply();
    set_uniforms_(GetProgramRef());
    GetProgramRef()->Apply();
  }
}