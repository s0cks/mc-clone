#ifndef MCC_FBO_FACTORY_H
#define MCC_FBO_FACTORY_H

#include "mcc/fbo/fbo.h"
#include "mcc/fbo/fbo_attachment.h"
#include "mcc/fbo/fbo_attachments.h"

namespace mcc::fbo {
  class FboFactory {
  protected:
    AttachmentSet attachments_;

    Fbo* CreateFbo(const FboId id) const;
  public:
    FboFactory() = default;
    virtual ~FboFactory() = default;

    virtual void Attach(Attachment* attachment) {
      attachments_.Insert(attachment);
    }

    virtual rx::observable<Fbo*> Create(const int num = 1) const;
  };
}

#endif //MCC_FBO_FACTORY_H