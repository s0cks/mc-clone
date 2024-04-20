#ifndef MCC_PROGRAM_SPEC_H
#error "Please #include <mcc/program/program_spec.h> instead."
#endif //MCC_PROGRAM_SPEC_H

#ifndef MCC_PROGRAM_SPEC_JSON_H
#define MCC_PROGRAM_SPEC_JSON_H

#include "mcc/program/program_document.h"

namespace mcc::program {
  class JsonProgramSpec : public ProgramSpec {
  protected:
    ProgramDocument doc_;

    inline const ProgramDocument& doc() const {
      return doc_;
    }
  public:
    explicit JsonProgramSpec(const uri::Uri& uri):
      ProgramSpec(),
      doc_(uri) {
    }
    ~JsonProgramSpec() override = default;

    std::string GetProgramName() const override {
      const auto& name = doc().GetNameProperty();
      MCC_ASSERT(name.IsString());
      return std::string(name.GetString(), name.GetStringLength());
    }

    rx::observable<ShaderId> GetProgramShaders() const override;
  };
}

#endif //MCC_PROGRAM_SPEC_JSON_H