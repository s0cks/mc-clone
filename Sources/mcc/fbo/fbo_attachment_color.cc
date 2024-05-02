#include "mcc/fbo/fbo_attachment.h"

#include <sstream>

#include "mcc/fbo/fbo.h"
#include "mcc/texture/texture_builder.h"
#include "mcc/texture/texture_factory.h"

#include "mcc/render/render_settings.h"

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
    builder.SetFilter(texture::kLinearFilter);
    builder.SetWrap(texture::kClampToEdgeWrap);
    builder.SetType(GL_UNSIGNED_BYTE);
    const auto texture = builder.Build()
      .as_blocking()
      .first();
    MCC_ASSERT(texture);
    return new ColorAttachment(texture, level);
  }

  ColorAttachment* ColorAttachment::NewDefaultResolution(const Level level) {
    const auto resolution = render::GetResolution();
    return New(texture::kRGB, texture::TextureSize(resolution.width(), resolution.height()), level);
  }
}