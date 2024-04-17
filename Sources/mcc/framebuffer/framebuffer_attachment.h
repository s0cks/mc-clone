#ifndef MCC_FRAMEBUFFER_ATTACHMENT_H
#define MCC_FRAMEBUFFER_ATTACHMENT_H

#include <vector>
#include "mcc/texture/texture.h"

namespace mcc::fbuff {
#define FOR_EACH_FRAMEBUFFER_ATTACHMENT_TYPE(V) \
  V(ColorBuffer)                                \
  V(RenderBuffer)                               \
  V(Picking)

#define FORWARD_DECLARE(Name) class Name##Attachment;
  FOR_EACH_FRAMEBUFFER_ATTACHMENT_TYPE(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  enum FrameBufferAttachmentType {
#define DEFINE_TYPE(Name) k##Name##Attachment,
    FOR_EACH_FRAMEBUFFER_ATTACHMENT_TYPE(DEFINE_TYPE)
#undef DEFINE_TYPE
    kNumberOfFrameBufferAttachmentTypes,
  };

  typedef GLuint FrameBufferAttachmentTarget;

  class FrameBufferAttachment {
  protected:
    bool enabled_;
    FrameBufferAttachmentTarget target_;

    FrameBufferAttachment(const bool enabled, const FrameBufferAttachmentTarget target):
      enabled_(enabled),
      target_(target) {
    }
    explicit FrameBufferAttachment(const FrameBufferAttachmentTarget target):
      FrameBufferAttachment(true, target) {
    }
  public:
    virtual ~FrameBufferAttachment() = default;
    virtual const char* name() const = 0;
    virtual FrameBufferAttachmentType type() const = 0;

    virtual bool IsEnabled() const {
      return enabled_;
    }

    FrameBufferAttachmentTarget target() const {
      return target_;
    }

#define DEFINE_TYPE_CAST(Name) \
    virtual Name##Attachment* As##Name##Attachment() { return nullptr; }
    FOR_EACH_FRAMEBUFFER_ATTACHMENT_TYPE(DEFINE_TYPE_CAST)
#undef DEFINE_TYPE_CAST

#define DEFINE_TYPE_CHECK(Name) \
    inline bool Is##Name##Attachment() const { return type() == k##Name##Attachment; }
    FOR_EACH_FRAMEBUFFER_ATTACHMENT_TYPE(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

  typedef std::vector<FrameBufferAttachment*> FrameBufferAttachmentList;

#define DEFINE_FRAMEBUFFER_ATTACHMENT(Name)                                                                     \
  public:                                                                                                       \
    const char* name() const override { return #Name; }                                                         \
    FrameBufferAttachmentType type() const override { return FrameBufferAttachmentType::k##Name##Attachment; }  \
    Name##Attachment* As##Name##Attachment() override { return this; }

  class ColorBufferAttachment : public FrameBufferAttachment {
  protected:
    TextureRef texture_;

    ColorBufferAttachment(const GLuint slot, TextureRef texture):
      FrameBufferAttachment(GL_COLOR_ATTACHMENT0 + slot),
      texture_(texture) {
    }
  public:
    ~ColorBufferAttachment() override = default;
    DEFINE_FRAMEBUFFER_ATTACHMENT(ColorBuffer);

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

  class PickingAttachment : public FrameBufferAttachment {
  protected:
    TextureRef texture_;

    PickingAttachment(const bool enabled, const GLuint slot, TextureRef texture):
      FrameBufferAttachment(GL_COLOR_ATTACHMENT0 + slot),
      texture_(texture) {
    }
  public:
    ~PickingAttachment() override = default;
    DEFINE_FRAMEBUFFER_ATTACHMENT(Picking);

    TextureRef GetTexture() const {
      return texture_;
    }

    GLuint slot() const {
      return target_ - GL_COLOR_ATTACHMENT0;
    }
  public:
    static inline PickingAttachment*
    New(const bool enabled,
        const uint32_t slot,
        const Dimension& size,
        const texture::PixelStoreAlignment& alignment = texture::kDefaultAlignment,
        const texture::TextureFilter& filter = texture::kNearestFilter,
        const texture::TextureWrap& wrap = texture::kDefaultWrap) {
      //TODO:
      // const auto texture = new texture::Texture(texture::k2D, true, true, false, GL_RGB32I, size, GL_RED_INTEGER , GL_UNSIGNED_INT, false, alignment, filter, wrap, NULL);
      // glTexImage2D(GL_TEXTURE_2D, 0, texture->internal_format(), size[0], size[1], 0, texture->format(), texture->type(), NULL);
      // CHECK_GL(FATAL);
      return new PickingAttachment(enabled, slot, TextureRef());
    }

    static inline PickingAttachment*
    New(const uint32_t slot,
        const Dimension& size,
        const texture::PixelStoreAlignment& alignment = texture::kDefaultAlignment,
        const texture::TextureFilter& filter = texture::kLinearFilter,
        const texture::TextureWrap& wrap = texture::kDefaultWrap) {
      return New(true, slot, size, alignment, filter, wrap);
    }
  };
}

#endif //MCC_FRAMEBUFFER_ATTACHMENT_H