#ifndef MCC_FBO_ATTACHMENT_H
#error "Please #include <mcc/fbo/fbo_attachment.h> instead."
#endif //MCC_FBO_ATTACHMENT_H

#ifndef MCC_FBO_ATTACHMENT_STENCIL_H
#define MCC_FBO_ATTACHMENT_STENCIL_H

#include "mcc/fbo/fbo_attachment.h"

namespace mcc::fbo {
  class StencilAttachment : public Attachment {
  protected:
    StencilAttachment() = default;
  public:
    ~StencilAttachment() override = default;
    DECLARE_FBO_ATTACHMENT(Stencil);
  public:
    static StencilAttachment* New();
  };
}

#endif //MCC_FBO_ATTACHMENT_STENCIL_H