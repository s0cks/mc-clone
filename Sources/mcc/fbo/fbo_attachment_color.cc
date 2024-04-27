#include "mcc/fbo/fbo_attachment.h"

#include <sstream>

#include "mcc/fbo/fbo.h"
#include "mcc/texture/texture_builder.h"
#include "mcc/texture/texture_factory.h"

namespace mcc::fbo {
  std::string ColorAttachment::ToString() const {
    std::stringstream ss;
    ss << "fbo::ColorAttachment(";
    ss << "texture=" << GetTexture()->GetTextureId();
    ss << ")";
    return ss.str();
  }

  void ColorAttachment::AttachTo(const FboTarget target) {
    Fbo::AttachTexture2D(target, GetAttachmentPoint(), GetTexture()->GetTextureId());
  }

  ColorAttachment* ColorAttachment::New(const texture::TextureFormat format,
                                        const texture::TextureSize& size,
                                        const Level level) {
    using namespace texture;
    TextureBuilder builder(k2D);
    builder.SetFormat(format);
    builder.SetInternalFormat(format);
    builder.SetSize(size);
    const auto texture = builder.Build()
      .as_blocking()
      .first();
    MCC_ASSERT(texture);
    return new ColorAttachment(texture, level);
  }
}