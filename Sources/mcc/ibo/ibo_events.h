#ifndef MCC_IBO_EVENTS_H
#define MCC_IBO_EVENTS_H

#include "mcc/event.h"
#include "mcc/ibo/ibo_id.h"

namespace mcc::ibo {
#define FOR_EACH_IBO_EVENT(V) \
  V(IboCreated)               \
  V(IboDestroyed)

#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_IBO_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class Ibo;
  class IboEvent : public Event {
  protected:
    const Ibo* ibo_;

    explicit IboEvent(const Ibo* ibo):
      Event(),
      ibo_(ibo) {
      MCC_ASSERT(ibo);
    }
  public:
    ~IboEvent() override = default;

    const Ibo* GetIbo() const {
      return ibo_;
    }

    IboId GetIboId() const;

#define DEFINE_TYPE_CHECK(Name)                                       \
    virtual Name##Event* As##Name##Event() { return nullptr; }        \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_IBO_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_IBO_EVENT(Name)                                                    \
  public:                                                                          \
    Name##Event* As##Name##Event() override { return this; }                       \
    const char* GetName() const override { return #Name; }                         \
    std::string ToString() const override;                                         \
    static inline bool                                                             \
    Filter(IboEvent* event) {                                                      \
      return event                                                                 \
          && event->Is##Name##Event();                                             \
    }                                                                              \
    static inline std::function<bool(IboEvent*)>                                   \
    FilterBy(const IboId id) {                                                     \
      return [id](IboEvent* event) {                                               \
        return event                                                               \
            && event->Is##Name##Event()                                            \
            && event->GetIboId() == id;                                            \
      };                                                                           \
    }                                                                              \
    static inline Name##Event*                                                     \
    Cast(IboEvent* event) {                                                        \
      MCC_ASSERT(event);                                                           \
      MCC_ASSERT(event->Is##Name##Event());                                        \
      return event->As##Name##Event();                                             \
    }

  class IboCreatedEvent : public IboEvent {
  public:
    explicit IboCreatedEvent(const Ibo* ibo):
      IboEvent(ibo) {
    }
    ~IboCreatedEvent() override = default;

    DECLARE_IBO_EVENT(IboCreated);
  };

  class IboDestroyedEvent : public IboEvent {
  public:
    explicit IboDestroyedEvent(const Ibo* ibo):
      IboEvent(ibo) {
    }
    ~IboDestroyedEvent() override = default;

    DECLARE_IBO_EVENT(IboDestroyed);
  };
}

#endif //MCC_IBO_EVENTS_H