#ifndef MCC_PROGRAM_SPEC_H
#error "Please #include <mcc/program/program_spec.h> instead."
#endif //MCC_PROGRAM_SPEC_H

#ifndef MCC_PROGRAM_SPEC_BUILDER_H
#define MCC_PROGRAM_SPEC_BUILDER_H

#include "mcc/shader/shader_id.h"

namespace mcc::program {
  class ProgramSpecBuilder {
  private:
    std::string name_;
    std::set<ShaderId> shaders_;
  public:
    explicit ProgramSpecBuilder(const std::string& name):
      name_(name),
      shaders_() {
    }
    ~ProgramSpecBuilder() = default;

    void Attach(const ShaderId shader) {
      MCC_ASSERT(shader::IsValidShaderId(shader));
      shaders_.insert(shader);
    }

    void Attach(rx::observable<ShaderId> shaders) {
      shaders.subscribe([this](const ShaderId& next) {
        return Attach(next);
      });
    }

    ProgramSpec* Build() const;
  };
}

#endif //MCC_PROGRAM_SPEC_BUILDER_H