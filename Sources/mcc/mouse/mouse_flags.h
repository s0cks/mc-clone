#ifndef MCC_MOUSE_FLAGS_H
#define MCC_MOUSE_FLAGS_H

#include "mcc/flags.h"

namespace mcc::mouse {
static constexpr const auto kDefaultMouseVisible = false;
DECLARE_bool(mouse_visible);
static constexpr const auto kDefaultCaptureMouse = false;
DECLARE_bool(capture_mouse);

  static inline bool
  ShouldCaptureMouse() {
    return FLAGS_fullscreen || FLAGS_capture_mouse;
  }

  static inline bool
  IsCursorVisible() {
    return FLAGS_mouse_visible || !ShouldCaptureMouse();
  }

  static inline bool
  IsCursorHidden() {
    return !IsCursorVisible();
  }
}

#endif //MCC_MOUSE_FLAGS_H