#ifndef MCC_VAO_EVENTS_H
#define MCC_VAO_EVENTS_H

#include "mcc/event.h"
#include "mcc/vao/vao_id.h"

namespace mcc::vao {
#define FOR_EACH_VAO_EVENT(V) \
  V(VaoCreated)               \
  V(VaoDestroyed)

#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_VAO_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class VaoEvent : public Event {
  protected:
    VaoId id_;

    explicit VaoEvent(const VaoId id):
      Event(),
      id_(id) {
    }
  public:
    ~VaoEvent() override = default;

    VaoId GetVaoId() const {
      return id_;
    }

#define DEFINE_TYPE_CHECK(Name)                                       \
    virtual Name##Event* As##Name##Event() { return nullptr; }        \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_VAO_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_VAO_EVENT(Name)                                 \
  public:                                                       \
    std::string ToString() const override;                      \
    const char* GetName() const override { return #Name; }      \
    Name##Event* As##Name##Event() override { return this; }    \
    static inline bool                                          \
    Filter(VaoEvent* event) {                                   \
      return event                                              \
          && event->Is##Name##Event();                          \
    }                                                           \
    static inline Name##Event*                                  \
    Cast(VaoEvent* event) {                                     \
      MCC_ASSERT(event);                                        \
      MCC_ASSERT(event->Is##Name##Event());                     \
      return event->As##Name##Event();                          \
    }

  class VaoCreatedEvent : public VaoEvent {
  public:
    explicit VaoCreatedEvent(const VaoId id):
      VaoEvent(id) {  
    }
    ~VaoCreatedEvent() override = default;
    DECLARE_VAO_EVENT(VaoCreated);
  };

  class VaoDestroyedEvent : public VaoEvent {
  public:
    explicit VaoDestroyedEvent(const VaoId id):
      VaoEvent(id) {
    }
    ~VaoDestroyedEvent() override = default;
    DECLARE_VAO_EVENT(VaoDestroyed);
  };
}

#endif //MCC_VAO_EVENTS_H