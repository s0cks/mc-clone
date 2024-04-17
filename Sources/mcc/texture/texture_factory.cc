#include "mcc/texture/texture_factory.h"

namespace mcc::texture {
  class TextureFactoryBindScope {
  protected:
    TextureId id_;
    TextureTarget target_;
    bool bound_;
  public:
    explicit TextureFactoryBindScope(const TextureTarget target):
      id_(kInvalidTextureId),
      target_(target),
      bound_(false) {
      glGenTextures(1, &id_);
      CHECK_GL(FATAL);
      Bind();
    }
    explicit TextureFactoryBindScope(const TextureFactory* factory):
      TextureFactoryBindScope(factory->target()) {
    }
    ~TextureFactoryBindScope() {
      Unbind();
    }

    void Bind() {
      glBindTexture(target(), id());
      CHECK_GL(FATAL);
    }

    void Unbind() {
      glBindTexture(target(), 0);
      CHECK_GL(FATAL);
    }

    bool bound() const {
      return bound_;
    }

    TextureId id() const {
      return id_;
    }

    TextureTarget target() const {
      return target_;
    }

    inline bool valid() const {
      return id() != kInvalidTextureId;
    }

    void Destroy() {
      Unbind();
      if(valid()) {
        glDeleteTextures(1, &id_);
        CHECK_GL(FATAL);
      }
    }
  };

  TextureId TextureFactory::Create() const {
    TextureFactoryBindScope scope(this);
    ApplyProperties();
    return scope.id();
  }

  TextureId TextureFactory::Create(img::Image* image) const {
    TextureFactoryBindScope scope(this);
    ApplyProperties();
    ApplyImage(image);
    return scope.id();
  }

  void TextureFactory2D::ApplyImage(img::Image* image) const {
    const auto& size = image->size();
    const auto width = size[0];
    const auto height = size[1];
    const auto pixels = image->data();
    glTexImage2D(target(), 0, image->type(), width, height, 0, image->type(), GL_UNSIGNED_BYTE, (const GLvoid*) pixels->bytes());
    CHECK_GL(FATAL); 
  }
}