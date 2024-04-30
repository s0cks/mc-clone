#ifndef MCC_RENDER_FLAGS_H
#define MCC_RENDER_FLAGS_H

#include "mcc/flags.h"
#include "mcc/resolution.h"

namespace mcc::render {
  static constexpr const auto kDefaultResolutionString = "1440x1080";
  DECLARE_string(resolution);

  static inline Resolution
  GetDefaultResolution() {
    return Resolution(FLAGS_resolution);
  }
}

#endif //MCC_RENDER_FLAGS_H