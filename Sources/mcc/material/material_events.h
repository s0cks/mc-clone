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
    const Material* material_;

    explicit MaterialEvent(const Material* material):
      Event(),
      material_(material) {
    }
  public:
    ~MaterialEvent() override = default;

    const Material* GetMaterial() const {
      return material_;
    }
    DEFINE_EVENT_PROTOTYPE(FOR_EACH_MATERIAL_EVENT);
  };

#define DECLARE_MATERIAL_EVENT(Name)                                    \
  DECLARE_EVENT_TYPE(MaterialEvent, Name)

  class MaterialCreatedEvent : public MaterialEvent {
  public:
    explicit MaterialCreatedEvent(const Material* material):
      MaterialEvent(material) {
    }
    ~MaterialCreatedEvent() override = default;
    DECLARE_MATERIAL_EVENT(MaterialCreated);
  };

  class MaterialDestroyedEvent : public MaterialEvent {
  public:
    explicit MaterialDestroyedEvent(const Material* material):
      MaterialEvent(material) {
    }
    ~MaterialDestroyedEvent() override = default;
    DECLARE_MATERIAL_EVENT(MaterialDestroyed);
  };
}

#endif //MCC_MATERIAL_EVENTS_H