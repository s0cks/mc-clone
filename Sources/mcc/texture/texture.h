#ifndef MCC_TEXTURE_H
#define MCC_TEXTURE_H

#include "mcc/gfx.h"
#include "mcc/platform.h"

namespace mcc::texture {
  typedef GLuint TextureId;

  static constexpr const TextureId kInvalidTextureId = 0;

  enum ActiveTexture : GLenum {
    kTexture0 = GL_TEXTURE0,
    kTexture1 = GL_TEXTURE1,
  };

  class Texture {
  protected:
    TextureId id_;
  public:
    constexpr Texture(const TextureId id = kInvalidTextureId):
      id_(id) {
    }
    explicit Texture(const bool generate):
      id_(kInvalidTextureId) {
      if(generate) {
        glGenTextures(1, &id_);
        CHECK_GL(FATAL);
      }
    }
    Texture(const Texture& rhs):
      id_(rhs.id_) {
    }
    ~Texture() = default;

    TextureId id() const {
      return id_;
    }

    void Bind(const ActiveTexture texture) const {
      glActiveTexture(texture);
      CHECK_GL(FATAL);
      glBindTexture(GL_TEXTURE_2D, id_);
      CHECK_GL(FATAL);
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

    void operator=(const Texture& rhs) {
      id_ = rhs.id_;
    }

    void operator=(const TextureId& rhs) {
      id_ = rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Texture& rhs) {
      stream << "Texture(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  public:
    static Texture LoadFrom(const std::string& filename);
  };
}

#endif //MCC_TEXTURE_H