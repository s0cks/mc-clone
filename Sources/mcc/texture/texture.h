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
    kCubeMap = GL_TEXTURE_CUBE_MAP,
  };

  class Texture : public gfx::Resource {
  protected:
    TextureId id_;
  public:
    constexpr Texture(const TextureId id = kInvalidTextureId):
      Resource(),
      id_(id) {
    }
    explicit Texture(const bool generate):
      Resource(),
      id_(kInvalidTextureId) {
      if(generate) {
        glGenTextures(1, &id_);
        CHECK_GL(FATAL);
      }
    }
    Texture(const Texture& rhs):
      Resource(),
      id_(rhs.id_) {
    }
    ~Texture() override = default;

    TextureId id() const {
      return id_;
    }

    void Bind() const override {
      glBindTexture(GL_TEXTURE_2D, id_);
      CHECK_GL(FATAL);
    }

    void Unbind() const override {
      glBindTexture(GL_TEXTURE_2D, kInvalidTextureId);
    }

    void Delete() override {
      glDeleteTextures(1, &id_);
      CHECK_GL(FATAL);
    }

    void Bind(const ActiveTexture id) const {
      glActiveTexture(id);
      CHECK_GL(FATAL);
      Bind();
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
    static Texture LoadFrom(std::string filename);
    static Texture LoadCubeMapFrom(const std::string& filename);
  };
}

#endif //MCC_TEXTURE_H