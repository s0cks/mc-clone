#ifndef MCC_FBO_ATTACHMENTS_H
#define MCC_FBO_ATTACHMENTS_H

#include <set>
#include "mcc/fbo/fbo_attachment.h"

namespace mcc::fbo {
  class AttachmentSet {
    struct AttachmentComparator {
      bool operator() (Attachment* lhs, Attachment* rhs) const;
    };
  protected:
    std::set<Attachment*, AttachmentComparator> attachments_;
  public:
    AttachmentSet() = default;
    virtual ~AttachmentSet() = default;

    virtual void Insert(Attachment* attachment);
    virtual void Remove(Attachment* attachment);
    virtual uint64_t GetNumberOfAttachments() const;
    virtual rx::observable<Attachment*> ToObservable() const;

    inline rx::observable<ColorAttachment*> GetAllColorAttachments() const {
      return ToObservable()
        .filter([](Attachment* att) {
          return att->IsColorAttachment();
        })
        .map([](Attachment* att) {
          return att->AsColorAttachment();
        });
    }

    inline rx::observable<DepthAttachment*> GetAllDepthAttachments() const {
      return ToObservable()
        .filter([](Attachment* att) {
          return att->IsDepthAttachment();
        })
        .map([](Attachment* att) {
          return att->AsDepthAttachment();
        });
    }

    inline rx::observable<StencilAttachment*> GetAllStencilAttachments() const {
      return ToObservable()
        .filter([](Attachment* att) {
          return att->IsStencilAttachment();
        })
        .map([](Attachment* att) {
          return att->AsStencilAttachment();
        });
    }
  };
}

#endif //MCC_FBO_ATTACHMENTS_H