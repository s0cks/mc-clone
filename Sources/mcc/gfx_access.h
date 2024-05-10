#ifndef MCC_GFX_ACCESS_H
#define MCC_GFX_ACCESS_H

#include "mcc/gfx.h"

namespace mcc::gfx {
  enum Access : GLenum {
    kReadOnly = GL_READ_ONLY,
    kWriteOnly = GL_WRITE_ONLY,
    kReadWrite = GL_READ_WRITE,
  };

  static inline constexpr const char*
  ToString(const Access& rhs) {
    switch(rhs) {
      case kReadOnly:
        return "ro";
      case kWriteOnly:
        return "wo";
      case kReadWrite:
        return "rw";
      default:
        return "unknown";
    }
  }

  static inline std::ostream&
  operator<<(std::ostream& stream, const Access& rhs) {
    return stream << ToString(rhs);
  }
}

#endif //MCC_GFX_ACCESS_H