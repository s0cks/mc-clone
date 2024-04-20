#ifndef MCC_TEXTURE_ID_H
#define MCC_TEXTURE_ID_H

#include "mcc/gfx.h"

namespace mcc {
  namespace texture {
    typedef GLuint TextureId;
    static constexpr const TextureId kInvalidTextureId = 0;
  }
  using texture::TextureId;
  using texture::kInvalidTextureId;
}

#endif //MCC_TEXTURE_ID_H