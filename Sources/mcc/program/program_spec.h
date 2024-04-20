#ifndef MCC_PROGRAM_SPEC_H
#define MCC_PROGRAM_SPEC_H

#include <string>
#include "mcc/rx.h"
#include "mcc/shader/shader_id.h"

namespace mcc::program {
  class ProgramSpec {
  protected:
    ProgramSpec() = default;
  public:
    virtual ~ProgramSpec() = default;
    virtual std::string GetProgramName() const = 0;
    virtual rx::observable<ShaderId> GetProgramShaders() const = 0;
  };
}

#include "mcc/program/program_spec_json.h"
#include "mcc/program/program_spec_builder.h"

#endif //MCC_PROGRAM_SPEC_H