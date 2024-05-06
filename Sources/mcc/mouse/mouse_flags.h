#ifndef MCC_MOUSE_FLAGS_H
#define MCC_MOUSE_FLAGS_H

#include "mcc/flags.h"

namespace mcc::mouse {
static constexpr const auto kDefaultMouseVisible = false;
DECLARE_bool(mouse_visible);

  static inline bool
  IsCursorVisible() {
    return FLAGS_mouse_visible;
  }

  static inline bool
  IsCursorHidden() {
    return !IsCursorVisible();
  }
}

#endif //MCC_MOUSE_FLAGS_H