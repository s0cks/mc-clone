#ifndef MCC_UBO_EVENTS_H
#define MCC_UBO_EVENTS_H

#include "mcc/event.h"
#include "mcc/common.h"
#include "mcc/ubo/ubo_id.h"

namespace mcc::ubo {
#define FOR_EACH_UBO_EVENT(V) \
  V(UboCreated)               \
  V(UboDestroyed)

  class Ubo;
  class UboEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_UBO_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class UboEvent : public Event {
  protected:
    const Ubo* ubo_;
  
    explicit UboEvent(const Ubo* ubo):
      Event(),
      ubo_(ubo) {
      MCC_ASSERT(ubo);
    }
  public:
    ~UboEvent() override = default;

    const Ubo* GetUbo() const {
      return ubo_;
    }

    UboId GetUboId() const;
    DEFINE_EVENT_PROTOTYPE(FOR_EACH_UBO_EVENT);
  };

#define DECLARE_UBO_EVENT(Name)                       \
  DECLARE_EVENT_TYPE(UboEvent, Name)                  \
  static inline std::function<bool(UboEvent*)>        \
  FilterBy(const UboId id) {                          \
    return [id](UboEvent* event) {                    \
      return event                                    \
          && event->Is##Name##Event()                 \
          && event->GetUboId() == id;                 \
    };                                                \
  }

  class UboCreatedEvent : public UboEvent {
  public:
    explicit UboCreatedEvent(const Ubo* ubo):
      UboEvent(ubo) {
    }
    ~UboCreatedEvent() override = default;
    DECLARE_UBO_EVENT(UboCreated);
  };

  class UboDestroyedEvent : public UboEvent {
  public:
    explicit UboDestroyedEvent(const Ubo* ubo):
      UboEvent(ubo) {  
    }
    ~UboDestroyedEvent() override = default;
    DECLARE_UBO_EVENT(UboDestroyed);
  };
}

#endif //MCC_UBO_EVENTS_H