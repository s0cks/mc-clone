#ifndef MCC_FBO_ATTACHMENT_H
#error "Please #include <mcc/fbo/fbo_attachment.h> instead."
#endif //MCC_FBO_ATTACHMENT_H

#ifndef MCC_FBO_ATTACHMENT_COLOR_H
#define MCC_FBO_ATTACHMENT_COLOR_H

#include "mcc/fbo/fbo_attachment.h"

namespace mcc::fbo {
  class ColorBufferAttachment : public FrameBufferAttachment {
  protected:
    TextureRef texture_;

    ColorBufferAttachment(const GLuint slot, TextureRef texture):
      FrameBufferAttachment(GL_COLOR_ATTACHMENT0 + slot),
      texture_(texture) {
    }
  public:
    ~ColorBufferAttachment() override = default;
    DEFINE_FBO_ATTACHMENT(ColorBuffer);

    TextureRef GetTexture() const {
      return texture_;
    }

    GLuint slot() const {
      return target_ - GL_COLOR_ATTACHMENT0;
    }

    static inline ColorBufferAttachment*
    New(const uint32_t slot, 
        const Dimension& size,
        const GLenum internal_format,
        const GLenum format,
        const GLenum type,
        const texture::PixelStoreAlignment& alignment = texture::kDefaultAlignment,
        const texture::TextureFilter& filter = texture::kLinearFilter,
        const texture::TextureWrap& wrap = texture::kDefaultWrap) {
      //TODO:
      // const auto texture = new texture::Texture(texture::k2D, true, true, false, internal_format, size, format, type, false, alignment, filter, wrap, NULL);
      // glTexImage2D(GL_TEXTURE_2D, 0, texture->internal_format(), size[0], size[1], 0, texture->format(), texture->type(), NULL);
      // CHECK_GL(FATAL);
      return new ColorBufferAttachment(slot, TextureRef());
    }

    static inline ColorBufferAttachment*
    NewHdr(const uint32_t slot,
           const Dimension& size,
           const texture::PixelStoreAlignment& alignment = texture::kDefaultAlignment,
           const texture::TextureFilter& filter = texture::kLinearFilter,
           const texture::TextureWrap& wrap = texture::kDefaultWrap) {
      return New(slot, size, GL_RGBA16F, GL_RGBA, GL_FLOAT, alignment, filter, wrap);
    }

    static inline ColorBufferAttachment*
    NewDefault(const uint32_t slot,
           const Dimension& size,
           const texture::PixelStoreAlignment& alignment = texture::kDefaultAlignment,
           const texture::TextureFilter& filter = texture::kLinearFilter,
           const texture::TextureWrap& wrap = texture::kDefaultWrap) {
      return NewHdr(slot, size, alignment, filter, wrap);
    }
  };
}

#endif //MCC_FBO_ATTACHMENT_COLOR_H