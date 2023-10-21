#ifndef MCC_TEXTURE_H
#define MCC_TEXTURE_H

#include "mcc/gfx.h"
#include "mcc/platform.h"

namespace mcc::texture {
  typedef GLuint TextureId;

  static constexpr const TextureId kInvalidTextureId = 0;

  class Texture {
  public:
    enum ActiveTexture : GLenum {
      kTexture0 = GL_TEXTURE0,
      kTexture1 = GL_TEXTURE1,
    };
  protected:
    TextureId id_;
  public:
    constexpr Texture(const TextureId id = kInvalidTextureId):
      id_(id) {
    }
    Texture(const Texture& rhs) = default;
    ~Texture() = default;

    TextureId id() const {
      return id_;
    }

    void Bind(const ActiveTexture texture) const {
      glActiveTexture(texture);
      glBindTexture(GL_TEXTURE_2D, id());
    }

    void Bind0() const {
      return Bind(kTexture0);
    }

    void Bind1() const {
      return Bind(kTexture1);
    }

    constexpr operator TextureId () const {
      return id_;
    }
  public:
    static Texture LoadFrom(const std::string& filename);
  };
}

#endif //MCC_TEXTURE_H