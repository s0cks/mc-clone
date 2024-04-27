#ifndef MCC_TEXTURE_SCOPE_H
#define MCC_TEXTURE_SCOPE_H

#include "mcc/texture/texture.h"

namespace mcc::texture {
  class TextureScope {
  protected:
    Texture* texture_;

    explicit TextureScope(Texture* texture):
      texture_(texture) {
    }
  public:
    virtual ~TextureScope() = default;

    Texture* GetTexture() const {
      return texture_;
    }

    TextureId GetTextureId() const {
      return GetTexture()->GetTextureId();
    }
    
    TextureTarget GetTextureTarget() const {
      return GetTexture()->GetTextureTarget();
    }
  };

  class TextureBindScope : public TextureScope {
  public:
    explicit TextureBindScope(Texture* texture):
      TextureScope(texture) {
      Bind();
    }
    ~TextureBindScope() override {
      Unbind();
    }

    void Bind() const {
      Texture::BindTexture(GetTextureTarget(), GetTextureId());
    }
    
    void Bind(const int32_t slot) const {
      Texture::ActiveTexture(slot);
      Bind();
    }

    void Unbind() const {
      Texture::UnbindTexture(GetTextureTarget());
    }
  };
}

#endif //MCC_TEXTURE_SCOPE_H