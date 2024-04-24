#ifndef MCC_TEXTURE_ID_H
#define MCC_TEXTURE_ID_H

#include "mcc/gfx.h"

namespace mcc {
  namespace texture {
    typedef GLuint TextureId;
    static constexpr const TextureId kInvalidTextureId = 0;

    static inline bool
    IsValidTextureId(const TextureId id) {
      return id != kInvalidTextureId;
    }
  }
  using texture::TextureId;
  using texture::kInvalidTextureId;
  using texture::IsValidTextureId;
}

#endif //MCC_TEXTURE_ID_H