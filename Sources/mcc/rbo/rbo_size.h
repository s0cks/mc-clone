#ifndef MCC_RBO_SIZE_H
#define MCC_RBO_SIZE_H

#include "mcc/gfx.h"

namespace mcc::rbo {
  typedef glm::i32vec2 RboSize;

  int32_t GetMaxRboSize();

  static inline bool
  IsValidRboSize(const RboSize& size) {
    const auto max = GetMaxRboSize();
    return size[0] >= 0
        && size[0] <= max
        && size[1] >= 0
        && size[1] <= max;
  }
}

#endif //MCC_RBO_SIZE_H