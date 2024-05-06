#ifndef MCC_TEXTURE_ID_H
#define MCC_TEXTURE_ID_H

#include "mcc/rx.h"
#include "mcc/gfx.h"

namespace mcc {
  namespace texture {
    typedef GLuint TextureId;
    static constexpr const TextureId kInvalidTextureId = 0;

    static inline bool
    IsInvalidTextureId(const TextureId id) {
      return id <= kInvalidTextureId;
    }

    static inline bool
    IsValidTextureId(const TextureId id) {
      return id != kInvalidTextureId;
    }

    void GenerateTextureIds(TextureId* ids, const int num);

    static inline TextureId
    GenerateTextureId() {
      TextureId id;
      GenerateTextureIds(&id, 1);
      return id;
    }

    rx::observable<TextureId> GenerateTextureIdsAsync(const int num);

    static inline rx::observable<TextureId>
    GenerateTextureIdAsync() {
      return GenerateTextureIdsAsync(1);
    }
  }
  using texture::TextureId;
  using texture::kInvalidTextureId;
  using texture::IsValidTextureId;
}

#endif //MCC_TEXTURE_ID_H