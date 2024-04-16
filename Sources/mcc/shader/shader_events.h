#ifndef MCC_SHADER_EVENTS_H
#define MCC_SHADER_EVENTS_H

#include "mcc/rx.h"
#include "mcc/event.h"
#include "mcc/resource/resource_event.h"
#include "mcc/shader/shader_constants.h"

namespace mcc::shader {
#define FOR_EACH_SHADER_EVENT(V) \
  V(ShaderCreated)               \
  V(ShaderDestroyed)

  class ShaderEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_SHADER_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class ShaderEvent : public Event {
  protected:
    ShaderId id_;

    explicit ShaderEvent(const ShaderId id):
      Event(),
      id_(id) {
    }
  public:
    ~ShaderEvent() override = default;

    ShaderId GetShaderId() const {
      return id_;
    }
  };

#define DECLARE_SHADER_EVENT(Name)                                  \
  public:                                                           \
    const char* GetName() const override { return #Name; }          \
    std::string ToString() const override;

  class ShaderCreatedEvent : public resource::ResourceCreatedEvent,
                             public ShaderEvent {
  public:
    explicit ShaderCreatedEvent(const ShaderId id):
      ShaderEvent(id),
      resource::ResourceCreatedEvent() {
    }
    ~ShaderCreatedEvent() override = default;
    DECLARE_SHADER_EVENT(ShaderCreated);
  };

  class ShaderDestroyedEvent : public resource::ResourceDestroyedEvent,
                               public ShaderEvent {
  public:
    explicit ShaderDestroyedEvent(const ShaderId id):
      ShaderEvent(id),
      resource::ResourceDestroyedEvent() {
    }
    ~ShaderDestroyedEvent() override = default;
    DECLARE_SHADER_EVENT(ShaderDestroyed);
  };
}

#endif //MCC_SHADER_EVENTS_H