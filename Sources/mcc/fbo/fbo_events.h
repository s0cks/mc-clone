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

#define DECLARE_FRAMEBUFFER_EVENT(Name)                                 \
  public:                                                               \
    const char* GetName() const override { return #Name; }              \
    std::string ToString() const override;                              \
    static inline bool                                                  \
    Filter(FboEvent* event) {                                           \
      return event                                                      \
          && event->Is##Name##Event();                                  \
    }                                                                   \
    static inline std::function<bool(FboEvent*)>                        \
    FilterBy(const FboId id) {                                          \
      return [id](FboEvent* event) {                                    \
        return event                                                    \
            && event->Is##Name##Event()                                 \
            && event->GetFboId() == id;                                 \
      };                                                                \
    }                                                                   \
    static inline Name##Event*                                          \
    Cast(FboEvent* event) {                                             \
      MCC_ASSERT(event);                                                \
      MCC_ASSERT(event->Is##Name##Event());                             \
      return event->As##Name##Event();                                  \
    }

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

#define DEFINE_TYPE_CHECK(Name)                                           \
    virtual Name##Event* As##Name##Event() { return nullptr; }            \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_FBO_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

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