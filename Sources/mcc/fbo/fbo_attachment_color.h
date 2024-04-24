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
    FboColorAttachment() = default;
  public:
    ~FboColorAttachment() override = default;
  };
}

#endif //MCC_FBO_ATTACHMENT_COLOR_H