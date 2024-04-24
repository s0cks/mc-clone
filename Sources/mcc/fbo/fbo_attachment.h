#ifndef MCC_FBO_ATTACHMENT_H
#define MCC_FBO_ATTACHMENT_H

#include <vector>
#include "mcc/fbo/fbo_target.h"

namespace mcc::fbo {
#define FOR_EACH_FBO_ATTACHMENT_TYPE(V) \
  V(Color)                              \
  V(Depth)                              \
  V(Stencil)

  class FboAttachment;
#define FORWARD_DECLARE(Name) class Fbo##Name##Attachment;
  FOR_EACH_FBO_ATTACHMENT_TYPE(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class FboAttachment {
  protected:
    FboAttachment() = default;

    virtual void AttachTo(const FboTarget target) = 0;
  public:
    virtual ~FboAttachment() = default;
    virtual std::string ToString() const = 0;
  };
}

#include "mcc/fbo/fbo_attachment_color.h"
#include "mcc/fbo/fbo_attachment_depth.h"
#include "mcc/fbo/fbo_attachment_stencil.h"

#endif //MCC_FBO_ATTACHMENT_H