#ifndef MCC_FRAMEBUFFER_ATTACHMENT_H
#define MCC_FRAMEBUFFER_ATTACHMENT_H

#include <vector>
#include "mcc/texture/texture.h"

namespace mcc {
  class FrameBufferAttachment {
  public:
    enum Type {
      kColorBuffer,
      kDepthBuffer,
    };
  protected:
    GLuint id_;

    FrameBufferAttachment(const GLuint id):
      id_(id) {
    }
  public:
    virtual ~FrameBufferAttachment() = default;
    virtual bool IsDrawBuffer() const { return false; }
    virtual Type GetType() const = 0;

    GLuint id() const {
      return id_;
    }
  };

  typedef std::vector<FrameBufferAttachment*> FrameBufferAttachmentList;

  class ColorBufferAttachment : public FrameBufferAttachment {
  protected:
    TextureRef texture_;

    ColorBufferAttachment(const GLuint slot, TextureRef texture):
      FrameBufferAttachment(GL_COLOR_ATTACHMENT0 + slot),
      texture_(texture) {
    }
  public:
    ~ColorBufferAttachment() override = default;

    Type GetType() const override {
      return Type::kColorBuffer;
    }

    bool IsDrawBuffer() const override {
      return true;
    }

    TextureRef GetTexture() const {
      return texture_;
    }

    GLuint slot() const {
      return id_ - GL_COLOR_ATTACHMENT0;
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
      const auto texture = new texture::Texture(texture::k2D, true, true, false, internal_format, size, format, type, false, alignment, filter, wrap, NULL);
      glTexImage2D(GL_TEXTURE_2D, 0, texture->internal_format(), size[0], size[1], 0, texture->format(), texture->type(), NULL);
      CHECK_GL(FATAL);
      const auto ptr = res::Pointer(res::Tag(res::kTextureType), (uword) texture);
      return new ColorBufferAttachment(slot, TextureRef(ptr));
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
    NewPicking(const uint32_t slot,
               const Dimension& size,
               const texture::PixelStoreAlignment& alignment = texture::kDefaultAlignment,
               const texture::TextureFilter& filter = texture::kLinearFilter,
               const texture::TextureWrap& wrap = texture::kDefaultWrap) {
      return New(slot, size, GL_RGB32UI, GL_RGB_INTEGER, GL_UNSIGNED_INT, alignment, filter, wrap);
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

#endif //MCC_FRAMEBUFFER_ATTACHMENT_H