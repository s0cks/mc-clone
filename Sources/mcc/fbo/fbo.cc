#include "mcc/fbo/fbo.h"

namespace mcc::fbo {
  using namespace texture;

  rx::subject<FboEvent*> events_;

  rx::observable<FboEvent*> OnFboEvent() {
    return events_.get_observable();
  }

  rx::observable<FboEvent*> OnFboEvent(const FboId id) {
    return OnFboEvent()
      .filter([id](FboEvent* event) {
        return event
            && event->GetFboId() == id;
      });
  }

  void Fbo::Destroy() {
    
  }

  std::string Fbo::ToString() const {
    std::stringstream ss;
    ss << "Fbo(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  Fbo* Fbo::New(const FboId id) {
    const auto fbo = new Fbo(id);
    MCC_ASSERT(fbo);
    PublishEvent<FboCreatedEvent>(fbo);
    return fbo;
  }

  void Fbo::PublishEvent(FboEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    return subscriber.on_next(event);
  }

  void Fbo::BindFbo(const FboId id) {
    MCC_ASSERT(IsValidFboId(id));
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    CHECK_GL(FATAL);
  }

  void Fbo::DeleteFbos(const FboId* ids, const int num_ids) {
    MCC_ASSERT(num_ids >= 1);
    glDeleteFramebuffers(num_ids, ids);
    CHECK_GL(FATAL);
  }

  static inline bool
  IsValidTextureTarget(const texture::TextureTarget target) {
    switch(target) {
      case texture::k2D:
        return true; //TODO: need to support cube maps at some point
      default:
        return false;
    }
  }

  void Fbo::AttachTexture(const FboTarget target,
                          const AttachmentPoint p,
                          const TextureTarget textarget,
                          const TextureId texid,
                          const int level) {
    MCC_ASSERT(IsValidTextureTarget(textarget));
    MCC_ASSERT(level == 0); // TODO: ??
    MCC_ASSERT(IsValidTextureId(texid));
    glFramebufferTexture2D(target, p, textarget, texid, level);
    CHECK_GL(FATAL);
  }
}