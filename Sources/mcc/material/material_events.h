#ifndef MCC_MATERIAL_EVENTS_H
#define MCC_MATERIAL_EVENTS_H

#include "mcc/event.h"
#include "mcc/common.h"

namespace mcc::material {
#define FOR_EACH_MATERIAL_EVENT(V) \
  V(MaterialCreated)               \
  V(MaterialDestroyed)

  class Material;
  class MaterialEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_MATERIAL_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class MaterialEvent : public Event {
  protected:
    MaterialEvent() = default;
  public:
    ~MaterialEvent() override = default;

#define DEFINE_TYPE_CHECK(Name)                                         \
    virtual Name##Event* As##Name##Event() { return nullptr; }          \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_MATERIAL_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_MATERIAL_EVENT(Name)                                    \
  public:                                                               \
    std::string ToString() const override;                              \
    const char* GetName() const override { return #Name; }              \
    Name##Event* As##Name##Event() override { return this; }            \
    static inline bool                                                  \
    Filter(MaterialEvent* event) {                                      \
      return event                                                      \
          && event->Is##Name##Event();                                  \
    }                                                                   \
    static inline Name##Event*                                          \
    Cast(MaterialEvent* event) {                                        \
      MCC_ASSERT(event);                                                \
      MCC_ASSERT(event->Is##Name##Event());                             \
      return event->As##Name##Event();                                  \
    }

  class MaterialCreatedEvent : public MaterialEvent {
  public:
    MaterialCreatedEvent() = default;
    ~MaterialCreatedEvent() override = default;
    DECLARE_MATERIAL_EVENT(MaterialCreated);
  };

  class MaterialDestroyedEvent : public MaterialEvent {
  public:
    MaterialDestroyedEvent() = default;
    ~MaterialDestroyedEvent() override = default;
    DECLARE_MATERIAL_EVENT(MaterialDestroyed);
  };
}

#endif //MCC_MATERIAL_EVENTS_H