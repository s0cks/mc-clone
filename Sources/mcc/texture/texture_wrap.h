#ifndef MCC_TEXTURE_WRAP_H
#define MCC_TEXTURE_WRAP_H

#include "mcc/gfx.h"
#include "mcc/texture/texture_target.h"

namespace mcc::texture {
    enum TextureWrapMode : GLenum {
    kRepeat = GL_REPEAT,
    kClampToEdge = GL_CLAMP_TO_EDGE,
    kClampToBorder = GL_CLAMP_TO_BORDER,

    kDefaultWrapMode = kClampToEdge,
  };

  struct TextureWrap {
    TextureWrapMode r;
    TextureWrapMode s;
    TextureWrapMode t;

    constexpr TextureWrap(const TextureWrapMode R,
                          const TextureWrapMode S,
                          const TextureWrapMode T):
      r(R),
      s(S),
      t(T) {
    }
    explicit constexpr TextureWrap(const TextureWrapMode mode = kDefaultWrapMode):
      TextureWrap(mode, mode, mode) {
    }
    TextureWrap(const TextureWrap& rhs) = default;
    ~TextureWrap() = default;
    TextureWrap& operator=(const TextureWrap& rhs) = default;

    void ApplyTo(const TextureTarget target) {
      glTexParameteri(target, GL_TEXTURE_WRAP_R, r);
      CHECK_GL(FATAL);
      glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
      CHECK_GL(FATAL);
      glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
      CHECK_GL(FATAL);
    }
  };

  static constexpr const auto kDefaultWrap = TextureWrap(kDefaultWrapMode);
  static constexpr const auto kClampToEdgeWrap = TextureWrap(kClampToEdge);
  static constexpr const auto kClampToBorderWrap = TextureWrap(kClampToBorder);
}

#endif //MCC_TEXTURE_WRAP_H