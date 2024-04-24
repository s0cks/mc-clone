#ifndef MCC_FBO_ATTACHMENT_H
#error "Please #include <mcc/fbo/fbo_attachment.h> instead."
#endif //MCC_FBO_ATTACHMENT_H

#ifndef MCC_FBO_ATTACHMENT_COLOR_H
#define MCC_FBO_ATTACHMENT_COLOR_H

#include "mcc/texture/texture.h"
#include "mcc/fbo/fbo_attachment.h"

namespace mcc::fbo {
  class FboColorAttachment : public FboAttachment {
  protected:
    TextureId id_;

    explicit FboColorAttachment(const TextureId id):
      FboAttachment(),
      id_(id) {
    }
  public:
    ~FboColorAttachment() override = default;

    TextureId GetTextureId() const {
      return id_;
    }
  public:
    static FboColorAttachment* New(const glm::i32vec2& size);
  };
}

#endif //MCC_FBO_ATTACHMENT_COLOR_H