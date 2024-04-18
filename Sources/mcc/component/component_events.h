#ifndef MCC_COMPONENT_EVENTS_H
#define MCC_COMPONENT_EVENTS_H

#include "mcc/event.h"
#include "mcc/common.h"

namespace mcc::component {
#define FOR_EACH_COMPONENT_EVENT(V) \
  V(ComponentRegistered)            \
  V(ComponentStateCreated)          \
  V(ComponentStateDestroyed)

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

  class Component;
  class ComponentEvent : public Event {
  protected:
    Component* component_;

    explicit ComponentEvent(Component* component):
      Event(),
      component_(component) {
    }
  public:
    ~ComponentEvent() override = default;

    Component* component() const {
      return component_;
    }

    virtual bool IsComponentStateEvent() const {
      return false;
    }

#define DEFINE_TYPE_CHECK(Name)                                             \
    virtual Name##Event* As##Name##Event() { return nullptr; }              \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_COMPONENT_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

  class ComponentRegisteredEvent : public ComponentEvent {
  public:
    explicit ComponentRegisteredEvent(Component* component):
      ComponentEvent(component) {
    }
    ~ComponentRegisteredEvent() override = default;
    DEFINE_COMPONENT_EVENT(ComponentRegistered);
  };

  class ComponentStateEvent : public ComponentEvent {
  protected:
    explicit ComponentStateEvent(Component* component):
      ComponentEvent(component) {
    }
  public:
    ~ComponentStateEvent() override = default;

    bool IsComponentStateEvent() const override {
      return true;
    }
  };

  class ComponentStateCreatedEvent : public ComponentStateEvent {
  protected:
    explicit ComponentStateCreatedEvent(Component* component):
      ComponentStateEvent(component) {
    }
  public:
    ~ComponentStateCreatedEvent() override = default;
    DEFINE_COMPONENT_EVENT(ComponentStateCreated);
  };

  class ComponentStateDestroyedEvent : public ComponentStateEvent {
  protected:
    explicit ComponentStateDestroyedEvent(Component* component):
      ComponentStateEvent(component) {
    }
  public:
    ~ComponentStateDestroyedEvent() override = default;
    DEFINE_COMPONENT_EVENT(ComponentStateDestroyed);
  };
}

#undef DEFINE_COMPONENT_EVENT

#endif //MCC_COMPONENT_EVENTS_H