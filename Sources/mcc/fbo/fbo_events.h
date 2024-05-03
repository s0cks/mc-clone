#ifndef MCC_FRAMEBUFFER_EVENTS_H
#define MCC_FRAMEBUFFER_EVENTS_H

#include "mcc/event.h"
#include "mcc/fbo/fbo_id.h"

namespace mcc::fbo {
#define FOR_EACH_FBO_EVENT(V) \
  V(FboCreated)               \
  V(FboDestroyed)

  class FboEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_FBO_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class Fbo;
  class FboEvent : public Event {
  protected:
    const Fbo* fbo_;

    explicit FboEvent(const Fbo* fbo):
      Event(),
      fbo_(fbo) {
    }
  public:
    ~FboEvent() override = default;

    const Fbo* GetFbo() const {
      return fbo_;
    }

    FboId GetFboId() const;
    DEFINE_EVENT_PROTOTYPE(FOR_EACH_FBO_EVENT);
  };

#define DECLARE_FRAMEBUFFER_EVENT(Name)                               \
  DECLARE_EVENT_TYPE(FboEvent, Name)                                  \
  static inline std::function<bool(FboEvent*)>                        \
  FilterBy(const FboId id) {                                          \
    return [id](FboEvent* event) {                                    \
      return event                                                    \
          && event->Is##Name##Event()                                 \
          && event->GetFboId() == id;                                 \
    };                                                                \
  }

  class FboCreatedEvent : public FboEvent {
  public:
    explicit FboCreatedEvent(const Fbo* fbo):
      FboEvent(fbo) {
    }
    ~FboCreatedEvent() override = default;
    DECLARE_FRAMEBUFFER_EVENT(FboCreated);
  };

  class FboDestroyedEvent : public FboEvent {
  public:
    explicit FboDestroyedEvent(const Fbo* fbo):
      FboEvent(fbo) {
    }
    ~FboDestroyedEvent() override = default;
    DECLARE_FRAMEBUFFER_EVENT(FboDestroyed);
  };
}

#endif //MCC_FRAMEBUFFER_EVENTS_H