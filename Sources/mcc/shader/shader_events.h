#ifndef MCC_SHADER_EVENTS_H
#define MCC_SHADER_EVENTS_H

#include "mcc/rx.h"
#include "mcc/event.h"
#include "mcc/resource/resource_event.h"

#include "mcc/shader/shader_id.h"

namespace mcc::shader {
#define FOR_EACH_SHADER_EVENT(V) \
  V(ShaderCreated)               \
  V(ShaderCompiled)              \
  V(ShaderDestroyed)

  class ShaderEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_SHADER_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class ShaderEvent : public Event {
  public:
    static inline std::function<bool(ShaderEvent*)>
    FilterBy(const ShaderId id) {
      return [id](ShaderEvent* event) {
        return event
            && event->GetShaderId() == id;
      };
    }
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

#define DEFINE_TYPE_CHECK(Name)                                         \
    virtual Name##Event* As##Name##Event() { return nullptr; }          \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_SHADER_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_SHADER_EVENT(Name)                                  \
  public:                                                           \
    const char* GetName() const override { return #Name; }          \
    std::string ToString() const override;                          \
    Name##Event* As##Name##Event() override { return this; }        \
  public:                                                           \
    static inline Name##Event*                                      \
    Cast(ShaderEvent* event) {                                      \
      return event ? event->As##Name##Event() : nullptr;            \
    }                                                               \
    static inline bool                                              \
    Filter(ShaderEvent* event) {                                    \
      return event                                                  \
          && event->Is##Name##Event();                              \
    }                                                               \
    static inline std::function<bool(ShaderEvent*)>                 \
    FilterBy(const ShaderId id) {                                   \
      return [id](ShaderEvent* event) {                             \
        return event                                                \
            && event->Is##Name##Event()                             \
            && event->GetShaderId() == id;                          \
      };                                                            \
    }

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

  class ShaderCompiledEvent : public ShaderEvent {
  protected:
    uint64_t total_ns_;
  public:
    explicit ShaderCompiledEvent(const ShaderId id,
                                 const uint64_t total_ns):
      ShaderEvent(id),
      total_ns_(total_ns) {
    }
    ~ShaderCompiledEvent() override = default;

    uint64_t total_ns() const {
      return total_ns_;
    }

    DECLARE_SHADER_EVENT(ShaderCompiled);
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