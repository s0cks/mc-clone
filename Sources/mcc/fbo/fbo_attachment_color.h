#ifndef MCC_FBO_ATTACHMENT_H
#error "Please #include <mcc/fbo/fbo_attachment.h> instead."
#endif //MCC_FBO_ATTACHMENT_H

#ifndef MCC_FBO_ATTACHMENT_COLOR_H
#define MCC_FBO_ATTACHMENT_COLOR_H

#include "mcc/texture/texture.h"
#include "mcc/fbo/fbo_attachment.h"

namespace mcc::fbo {
  using texture::Texture;
  class ColorAttachment : public Attachment {
  protected:
    Texture* texture_;

    explicit ColorAttachment(Texture* texture):
      Attachment(),
      texture_(texture) {
    }
  public:
    ~ColorAttachment() override = default;

    virtual Texture* GetTexture() const {
      return texture_;
    }

    DECLARE_FBO_ATTACHMENT(Color);
  public:
    static ColorAttachment* New(const glm::i32vec2& size);
  };
}

#endif //MCC_FBO_ATTACHMENT_COLOR_H