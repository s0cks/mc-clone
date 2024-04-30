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

#define DEFINE_TYPE_CHECK(Name)                                                     \
    virtual Name##Event* As##Name##Event() { return nullptr; }                      \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_UBO_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_UBO_EVENT(Name)                                                     \
  public:                                                                           \
    std::string ToString() const override;                                          \
    const char* GetName() const override { return #Name; }                          \
    Name##Event* As##Name##Event() override { return this; }                        \
    static inline bool                                                              \
    Filter(UboEvent* event) {                                                       \
      return event                                                                  \
          && event->Is##Name##Event();                                              \
    }                                                                               \
    static inline Name##Event*                                                      \
    Cast(UboEvent* event) {                                                         \
      MCC_ASSERT(event);                                                            \
      MCC_ASSERT(event->Is##Name##Event());                                         \
      return event->As##Name##Event();                                              \
    }                                                                               \
    static inline std::function<bool(UboEvent*)>                                    \
    FilterBy(const UboId id) {                                                      \
      return [id](UboEvent* event) {                                                \
        return event                                                                \
            && event->Is##Name##Event()                                             \
            && event->GetUboId() == id;                                             \
      };                                                                            \
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