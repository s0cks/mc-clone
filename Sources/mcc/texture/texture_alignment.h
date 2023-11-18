#ifndef MCC_TEXTURE_ALIGNMENT_H
#define MCC_TEXTURE_ALIGNMENT_H

#include "mcc/gfx.h"

namespace mcc::texture {
  enum TextureAlignment : GLint {
    kNone = 0,
    k1 = 1,
    k2 = 2,
    k4 = 4,
    k8 = 8,

    kByteAlignment = k1,
    kRowAlignment = k2,
    kWordAlignment = k4,
    kDoubleWordAlignment = k8,


    kDefaultPackAlignment = k4,
    kDefaultUnpackAlignment = k4,
  };

  struct PixelStoreAlignment {
    TextureAlignment pack;
    TextureAlignment unpack;

    constexpr PixelStoreAlignment(const TextureAlignment pack_alignment,
                                  const TextureAlignment unpack_alignment):
                        pack(pack_alignment),
                        unpack(unpack_alignment) {
    }
    constexpr PixelStoreAlignment():
      PixelStoreAlignment(kDefaultPackAlignment, kDefaultUnpackAlignment) {
    }
    PixelStoreAlignment(const PixelStoreAlignment& rhs) = default;
    ~PixelStoreAlignment() = default;

    PixelStoreAlignment& operator=(const PixelStoreAlignment& rhs) = default;

    void Apply() {
      glPixelStorei(GL_PACK_ALIGNMENT, pack);
      CHECK_GL(FATAL);
      glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
      CHECK_GL(FATAL);
    }
  };

  static constexpr const auto kDefaultAlignment = PixelStoreAlignment();
}

#endif //MCC_TEXTURE_ALIGNMENT_H