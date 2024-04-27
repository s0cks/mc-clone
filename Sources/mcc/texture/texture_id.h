#ifndef MCC_TEXTURE_ID_H
#define MCC_TEXTURE_ID_H

#include "mcc/rx.h"
#include "mcc/gfx.h"

namespace mcc {
  namespace texture {
    typedef GLuint TextureId;
    static constexpr const TextureId kInvalidTextureId = 0;

    static inline bool
    IsValidTextureId(const TextureId id) {
      return id != kInvalidTextureId;
    }

    rx::observable<TextureId> GenerateTextureIds(const int num);

    static inline rx::observable<TextureId>
    GenerateTextureId() {
      return GenerateTextureIds(1);
    }
  }
  using texture::TextureId;
  using texture::kInvalidTextureId;
  using texture::IsValidTextureId;
}

#endif //MCC_TEXTURE_ID_H