#ifndef MCC_COMPONENT_EVENTS_H
#define MCC_COMPONENT_EVENTS_H

#include "mcc/event.h"
#include "mcc/common.h"

namespace mcc::component {
#define FOR_EACH_COMPONENT_EVENT(V) \
  V(ComponentRegistered)            \
  V(ComponentCreated)               \
  V(ComponentDestroyed)

  class ComponentEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_COMPONENT_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

#define DEFINE_COMPONENT_EVENT(Name)                                        \
  public:                                                                   \
    Name##Event* As##Name##Event() override { return this; }                \
    const char* GetName() const override { return #Name; }                  \
    std::string ToString() const override;                                  \
    static inline bool                                                      \
    Filter(ComponentEvent* event) {                                         \
      MCC_ASSERT(event);                                                    \
      return event->Is##Name##Event();                                      \
    }                                                                       \
    static inline Name##Event*                                              \
    Cast(ComponentEvent* event) {                                           \
      MCC_ASSERT(event);                                                    \
      MCC_ASSERT(event->Is##Name##Event());                                 \
      return event->As##Name##Event();                                      \
    }

  class ComponentEvent : public Event {
  protected:
    ComponentEvent() = default;
  public:
    ~ComponentEvent() override = default;

#define DEFINE_TYPE_CHECK(Name)                                             \
    virtual Name##Event* As##Name##Event() { return nullptr; }              \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_COMPONENT_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

  class ComponentRegisteredEvent : public ComponentEvent {
  public:
    ComponentRegisteredEvent() = default;
    ~ComponentRegisteredEvent() override = default;
    DEFINE_COMPONENT_EVENT(ComponentRegistered);
  };

  class ComponentCreatedEvent : public ComponentEvent {
  public:
    ComponentCreatedEvent() = default;
    ~ComponentCreatedEvent() override = default;
    DEFINE_COMPONENT_EVENT(ComponentCreated);
  };

  class ComponentDestroyedEvent : public ComponentEvent {
  public:
    ComponentDestroyedEvent() = default;
    ~ComponentDestroyedEvent() override = default;
    DEFINE_COMPONENT_EVENT(ComponentDestroyed);
  };
}

#undef DEFINE_COMPONENT_EVENT

#endif //MCC_COMPONENT_EVENTS_H