#ifndef MCC_WINDOW_FLAGS_H
#define MCC_WINDOW_FLAGS_H

#include "mcc/flags.h"

namespace mcc {
  static constexpr const auto kDefaultWindowWidth = 0;
  DECLARE_int32(window_width);

  static constexpr const auto kDefaultWindowHeight = 0;
  DECLARE_int32(window_height);
}

#endif //MCC_WINDOW_FLAGS_H