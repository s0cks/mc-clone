#ifndef MCC_PROGRAM_LINKER_H
#define MCC_PROGRAM_LINKER_H

#include <vector>
#include "mcc/rx.h"
#include "mcc/shader/shader_id.h"
#include "mcc/program/program_id.h"
#include "mcc/program/program_spec.h"
#include "mcc/program/program_status.h"

namespace mcc::program {
  class ProgramLinker {
  protected:
    ProgramId program_;
    std::set<ShaderId> attached_;

    inline ProgramId program() const {
      return program_;
    }

    void Detach(const ShaderId id, const bool remove = true);

    inline void DetachAll() {
      for(const auto& shader : attached_) {
        Detach(shader, false);
      }
      attached_.clear();
    }
  public:
    explicit ProgramLinker(const ProgramId program):
      program_(program),
      attached_() {
      MCC_ASSERT(IsValidProgramId(program));
    }
    virtual ~ProgramLinker() = default;

    bool IsAttached(const ShaderId id) const {
      MCC_ASSERT(shader::IsValidShaderId(id));
      const auto pos = attached_.find(id);
      return pos != attached_.end();
    }

    void Attach(const ShaderId id);

    void Attach(rx::observable<ShaderId> shaders) {
      shaders.as_blocking()
        .subscribe([this](const ShaderId shader) {
          return Attach(shader);
        });
    }

    ProgramLinkStatus LinkProgram();
  public:
    static inline ProgramLinkStatus
    Link(const ProgramId program, rx::observable<ShaderId> shaders) {
      ProgramLinker linker(program);
      linker.Attach(shaders);
      return linker.LinkProgram();
    }

    static inline ProgramLinkStatus
    Link(const ProgramId program, ProgramSpec* spec) {
      MCC_ASSERT(spec);
      return Link(program, spec->GetProgramShaders());
    }
  };
}

#endif //MCC_PROGRAM_LINKER_H