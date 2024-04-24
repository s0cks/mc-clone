#ifndef MCC_FBO_ATTACHMENT_H
#error "Please #include <mcc/fbo/fbo_attachment.h> instead."
#endif //MCC_FBO_ATTACHMENT_H

#ifndef MCC_FBO_ATTACHMENT_DEPTH_H
#define MCC_FBO_ATTACHMENT_DEPTH_H

#include "mcc/fbo/fbo_attachment.h"

namespace mcc::fbo {
  class DepthAttachment : public Attachment {
  protected:
    DepthAttachment() = default;
  public:
    ~DepthAttachment() override = default;
    DECLARE_FBO_ATTACHMENT(Depth);
  public:
    static DepthAttachment* New();
  };
}

#endif //MCC_FBO_ATTACHMENT_DEPTH_H