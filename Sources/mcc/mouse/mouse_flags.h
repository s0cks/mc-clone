
#ifndef MCC_MOUSE_FLAGS_H
#define MCC_MOUSE_FLAGS_H

#include <fmt/format.h>
#include "mcc/flags.h"

namespace mcc::mouse {
static constexpr const auto kDefaultCursorsDir = "";
DECLARE_string(cursors_dir);

  static inline std::string
  GetCursorsDir() {
    if(!FLAGS_cursors_dir.empty())
      return FLAGS_cursors_dir;
    return fmt::format("{0:s}/cursors", FLAGS_resources);
  }
}

#endif //MCC_MOUSE_FLAGS_H