#include "mcc/texture/texture_builder.h"
#include "mcc/texture/texture.h"

namespace mcc::texture {
  TextureId TextureBuilderBase::Build() const {
    const auto id = GenerateTextureId();
    MCC_ASSERT(IsValidTextureId(id));
    Texture::BindTexture(GetTarget(), id);
    filter_.ApplyTo(GetTarget());
    wrap_.ApplyTo(GetTarget());
    InitTexture(id);
    Texture::UnbindTexture(GetTarget());
    return id;
  }
  
  rx::observable<TextureId> TextureBuilderBase::BuildAsync() const {
    return rx::observable<>::create<TextureId>([this](rx::subscriber<TextureId> s) {
      const auto id = Build();
      if(IsInvalidTextureId(id)) {
        const auto err = fmt::format("build returned invalid TextureId: {0:d}", id);
        return s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
      }
      
      s.on_next(id);
      s.on_completed();
    });
  }

  void TextureBuilder::InitTexture(const TextureId id) const {
    MCC_ASSERT(IsValidTextureId(id));
    switch(GetTarget()) {
      case k2D: {
        glTexImage2D(GetTarget(), GetLevel(), GetInternalFormat(), GetWidth(), GetHeight(), GetBorder(), GetFormat(), GetType(), GetData());
        CHECK_GL(FATAL);
        break;
      }
      default: {
        DLOG(ERROR) << "unknown TextureTarget: " << GetTarget();
        break;
      }
    }
  }
}