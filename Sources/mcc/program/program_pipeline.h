#ifndef MCC_PROGRAM_PIPELINE_H
#define MCC_PROGRAM_PIPELINE_H

#include "mcc/pipeline.h"
#include "mcc/program/program.h"

namespace mcc::program {
  class ProgramPipeline : public Pipeline {
  protected:
    ProgramRef ref_;

    explicit ProgramPipeline(ProgramRef ref):
      Pipeline(),
      ref_(ref) {
    }
  public:
    ~ProgramPipeline() override = default;

    inline const ProgramRef& GetProgramRef() const {
      return ref_;
    }

    inline ProgramRef& GetProgramRef() {
      return ref_;
    }
  };

  class ApplyProgramPipeline : public ProgramPipeline {
  public:
    typedef std::function<void(const ProgramRef&)> SetUniformsCallback;
    static const SetUniformsCallback kDoNothing;
  private:
    SetUniformsCallback set_uniforms_;

    ApplyProgramPipeline(ProgramRef ref, SetUniformsCallback callback):
      ProgramPipeline(ref),
      set_uniforms_(callback) {
    }
  public:
    ~ApplyProgramPipeline() override = default;

    const char* GetName() const override {
      return "ApplyProgram";
    }

    bool Apply() override;
  public:
    static inline ApplyProgramPipeline*
    New(const ProgramRef& ref, SetUniformsCallback callback = kDoNothing) {
      return new ApplyProgramPipeline(ref, callback);
    }
  };
}

#endif //MCC_PROGRAM_PIPELINE_H