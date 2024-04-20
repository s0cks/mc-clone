#include "mcc/program/program_spec.h"

namespace mcc::program {
  rx::observable<ShaderId> JsonProgramSpec::GetProgramShaders() const {
    return rx::observable<>::create<ShaderId>([this](rx::subscriber<ShaderId> s) {
      
    });
  }
}