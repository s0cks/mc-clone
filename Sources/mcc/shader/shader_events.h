#ifndef MCC_SHADER_EVENTS_H
#define MCC_SHADER_EVENTS_H

#include "mcc/rx.h"
#include "mcc/event.h"
#include "mcc/resource/resource_event.h"
#include "mcc/shader/shader_id.h"

namespace mcc::shader {
#define FOR_EACH_SHADER_EVENT(V) \
  V(ShaderCreated)               \
  V(ShaderDestroyed)

  class Shader;
  class ShaderEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_SHADER_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class ShaderEvent : public Event {
  protected:
    const Shader* shader_;

    explicit ShaderEvent(const Shader* shader):
      Event(),
      shader_(shader) {
      MCC_ASSERT(shader);
    }
  public:
    ~ShaderEvent() override = default;

    const Shader* GetShader() const {
      return shader_;
    }

    ShaderId GetShaderId() const;

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
    explicit ShaderCreatedEvent(const Shader* shader):
      ShaderEvent(shader),
      resource::ResourceCreatedEvent() {
    }
    ~ShaderCreatedEvent() override = default;
    DECLARE_SHADER_EVENT(ShaderCreated);
  };

  class ShaderDestroyedEvent : public resource::ResourceDestroyedEvent,
                               public ShaderEvent {
  public:
    explicit ShaderDestroyedEvent(const Shader* shader):
      ShaderEvent(shader),
      resource::ResourceDestroyedEvent() {
    }
    ~ShaderDestroyedEvent() override = default;
    DECLARE_SHADER_EVENT(ShaderDestroyed);
  };
}

#endif //MCC_SHADER_EVENTS_H