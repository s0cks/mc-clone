#include "mcc/texture/texture_builder.h"

namespace mcc::texture {
  Texture* TextureBuilder::InitTexture(const TextureId id) const {
    MCC_ASSERT(IsValidTextureId(id));
    Texture::BindTexture(target_, id);
    filter_.ApplyTo(target_);
    wrap_.ApplyTo(target_);
    glTexImage2D(target_, level_, internal_format_, size_[0], size_[1], border_, format_, type_, NULL);
    CHECK_GL(FATAL);
    Texture::UnbindTexture(target_);
    return new Texture2D(id);
  }

  rx::observable<Texture*> TextureBuilder::Build(const int num) const {
    return GenerateTextureIds(1)
      .map([this](const TextureId id) {
        return InitTexture(id);
      });
  }
}