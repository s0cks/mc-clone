#ifndef MCC_FBO_ATTACHMENT_H
#error "Please #include <mcc/fbo/fbo_attachment.h> instead."
#endif //MCC_FBO_ATTACHMENT_H

#ifndef MCC_FBO_ATTACHMENT_COLOR_H
#define MCC_FBO_ATTACHMENT_COLOR_H

#include "mcc/texture/texture.h"
#include "mcc/fbo/fbo_attachment.h"
#include "mcc/texture/texture_format.h"

namespace mcc::fbo {
  using texture::Texture;
  class ColorAttachment : public Attachment {
  public:
    enum Level : GLenum {
#define DEFINE_LEVEL(Name, GlValue) k##Name = (GlValue),
      FOR_EACH_FBO_COLOR_ATTACHMENT_POINT(DEFINE_LEVEL)
#undef DEFINE_LEVEL
      kMaxLevels = GL_MAX_COLOR_ATTACHMENTS,
    };
  protected:
    Texture* texture_;
    Level level_;

    explicit ColorAttachment(Texture* texture, const Level level):
      Attachment(),
      texture_(texture),
      level_(level) {
    }

    void AttachTo(const FboTarget target = kDefaultTarget) override;
  public:
    ~ColorAttachment() override = default;

    virtual Texture* GetTexture() const {
      return texture_;
    }

    virtual Level GetLevel() const {
      return level_;
    }

    AttachmentPoint GetAttachmentPoint() const override {
      return static_cast<AttachmentPoint>(GetLevel());
    }

    DECLARE_FBO_ATTACHMENT(Color);
  public:
    static ColorAttachment* New(const texture::TextureFormat format, const texture::TextureSize& size, const Level level);
  };
}

#endif //MCC_FBO_ATTACHMENT_COLOR_H