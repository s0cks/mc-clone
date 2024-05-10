#ifndef MCC_SHADER_ID_H
#define MCC_SHADER_ID_H

#include "mcc/gfx.h"

namespace mcc {
  namespace shader {
    typedef GLuint ShaderId;
    static constexpr const ShaderId kInvalidShaderId = 0;

    static inline bool
    IsInvalidShaderId(const ShaderId id) {
      return id == kInvalidShaderId;
    }

    static inline bool
    IsValidShaderId(const ShaderId id) {
      return id != kInvalidShaderId;
    }
  }
  using shader::ShaderId;
  using shader::kInvalidShaderId;
  using shader::IsValidShaderId;
  using shader::IsInvalidShaderId;
}

#endif //MCC_SHADER_ID_H