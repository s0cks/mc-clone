
#ifndef MCC_MOUSE_FLAGS_H
#define MCC_MOUSE_FLAGS_H

#include <fmt/format.h>
#include "mcc/flags.h"

namespace mcc::mouse {
static constexpr const auto kDefaultMouseVisible = false;
DECLARE_bool(mouse_visible);
static constexpr const auto kDefaultCaptureMouse = false;
DECLARE_bool(capture_mouse);
static constexpr const auto kDefaultCursorsDir = "";
DECLARE_string(cursors_dir);

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

  static inline std::string
  GetCursorsDir() {
    if(!FLAGS_cursors_dir.empty())
      return FLAGS_cursors_dir;
    return fmt::format("{0:s}/cursors", FLAGS_resources);
  }
}

#endif //MCC_MOUSE_FLAGS_H