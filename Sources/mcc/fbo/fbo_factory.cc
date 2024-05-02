#include "mcc/fbo/fbo_factory.h"

#include "mcc/fbo/fbo.h"
#include "mcc/fbo/fbo_status.h"

namespace mcc::fbo {
  rx::observable<Fbo*> FboFactory::Create(const int num) const {
    return GenerateFboId(num)
      .filter(IsValidFboId)
      .map([this](FboId id) {
        return CreateFbo(id);
      });
  }

  Fbo* FboFactory::CreateFbo(const FboId id) const {
    MCC_ASSERT(IsValidFboId(id));
    Fbo::BindFbo(id);
    DLOG(INFO) << "attaching " << attachments_.GetNumberOfAttachments() << " attachments....";
    for(const auto& attachment : attachments_) {
      DLOG(INFO) << "attaching " << attachment->ToString() << "....";
      attachment->AttachTo();
    }

    FboStatus status;
    LOG_IF(FATAL, !status) << "failed to initialize framebuffer: " << status;
    Fbo::BindDefaultFbo();
    return Fbo::New(id, attachments_);
  }
}