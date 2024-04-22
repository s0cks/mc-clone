#ifndef MCC_IBO_ID_H
#define MCC_IBO_ID_H

#include "mcc/buffer_object.h"

namespace mcc {
  namespace ibo {
    typedef BufferObjectId IboId;
    static constexpr const IboId kDefaultIboId = 0;
    static constexpr const IboId kInvalidIboId = -1;

    static inline bool
    IsValidIboId(const IboId id) {
      return id != kInvalidIboId;
    }
  }
  using ibo::IboId;
  using ibo::kInvalidIboId;
  using ibo::kDefaultIboId;
  using ibo::IsValidIboId;
}

#endif //MCC_IBO_ID_H