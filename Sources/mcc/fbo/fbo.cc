#include "mcc/fbo/fbo.h"

namespace mcc::fbo {
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
}