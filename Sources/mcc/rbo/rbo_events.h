#ifndef MCC_RBO_EVENTS_H
#define MCC_RBO_EVENTS_H

#include <functional>
#include "mcc/event.h"
#include "mcc/rbo/rbo_id.h"

namespace mcc::rbo {
#define FOR_EACH_RBO_EVENT(V) \
  V(RboCreated)               \
  V(RboDestroyed)

  class Rbo;
  class RboEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_RBO_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class RboEvent : public Event {
  protected:
    const Rbo* rbo_;

    explicit RboEvent(const Rbo* rbo):
      Event(),
      rbo_(rbo) {
    }
  public:
    ~RboEvent() override = default;

    const Rbo* GetRbo() const {
      return rbo_;
    }

    RboId GetRboId() const;

#define DEFINE_TYPE_CHECK(Name)                                                   \
    virtual Name##Event* As##Name##Event() { return nullptr; }                    \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_RBO_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_RBO_EVENT(Name)                                                   \
  public:                                                                         \
    std::string ToString() const override;                                        \
    const char* GetName() const override { return #Name; }                        \
    Name##Event* As##Name##Event() override { return this; }                      \
    static inline Name##Event*                                                    \
    Cast(RboEvent* event) {                                                       \
      MCC_ASSERT(event);                                                          \
      MCC_ASSERT(event->Is##Name##Event());                                       \
      return event->As##Name##Event();                                            \
    }                                                                             \
    static inline bool                                                            \
    Filter(RboEvent* event) {                                                     \
      return event                                                                \
          && event->Is##Name##Event();                                            \
    }                                                                             \
    static inline std::function<bool(RboEvent*)>                                  \
    FilterBy(const RboId id) {                                                    \
      return [id](RboEvent* event) {                                              \
        return event                                                              \
            && event->Is##Name##Event()                                           \
            && event->GetRboId() == id;                                           \
      };                                                                          \
    }

  class RboCreatedEvent : public RboEvent {
  public:
    explicit RboCreatedEvent(const Rbo* rbo):
      RboEvent(rbo) {
    }
    ~RboCreatedEvent() override = default;
    DECLARE_RBO_EVENT(RboCreated);
  };

  class RboDestroyedEvent : public RboEvent {
  public:
    explicit RboDestroyedEvent(const Rbo* rbo):
      RboEvent(rbo) {
    }
    ~RboDestroyedEvent() override = default;
    DECLARE_RBO_EVENT(RboDestroyed);
  };
}

#endif //MCC_RBO_EVENTS_H