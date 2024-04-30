#ifndef MCC_GFX_BUFFER_ID_H
#define MCC_GFX_BUFFER_ID_H

#include "mcc/gfx.h"

namespace mcc {
  namespace gfx {
    typedef GLuint BufferObjectId;
    static constexpr const BufferObjectId kInvalidBufferObjectId = 0;

    static inline bool
    IsValidBufferObjectId(const BufferObjectId id) {
      return id >= 0;
    }

    static inline bool
    IsInvalidBufferObjectId(const BufferObjectId id) {
      return id == kInvalidBufferObjectId;
    }
  }
  using gfx::BufferObjectId;
  using gfx::kInvalidBufferObjectId;
  using gfx::IsValidBufferObjectId;
  using gfx::IsInvalidBufferObjectId;
}

#endif //MCC_GFX_BUFFER_ID_H