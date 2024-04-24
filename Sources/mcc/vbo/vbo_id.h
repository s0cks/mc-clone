#ifndef MCC_VBO_ID_H
#define MCC_VBO_ID_H

#include "mcc/rx.h"
#include "mcc/gfx.h"

namespace mcc {
  namespace vbo {
    typedef GLuint VboId;
    static constexpr const VboId kInvalidVboId = -1;
    static constexpr const VboId kDefaultVboId = 0;

    static inline bool
    IsDefaultVboId(const VboId id) {
      return id == kDefaultVboId;
    }

    static inline bool
    IsInvalidVboId(const VboId id) {
      return id == kInvalidVboId;
    }

    static inline bool
    IsValidVboId(const VboId id) {
      return id >= kDefaultVboId;
    }

    rx::observable<VboId> GenerateVboId(const int num = 1);
  }
  using vbo::VboId;
  using vbo::kInvalidVboId;
}

#endif //MCC_VBO_ID_H