#ifndef MCC_PROGRAM_ID_H
#define MCC_PROGRAM_ID_H

#include "mcc/gfx.h"

namespace mcc {
  namespace program {
    typedef GLuint ProgramId;
    static constexpr const ProgramId kInvalidProgramId = 0;

    static inline bool
    IsValidProgramId(const ProgramId id) {
      return id != kInvalidProgramId;
    }
  }
  using program::ProgramId;
  using program::kInvalidProgramId;
}

#endif //MCC_PROGRAM_ID_H