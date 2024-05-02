#ifndef MCC_SHADER_COMPILER_EVENTS_H
#define MCC_SHADER_COMPILER_EVENTS_H

#include "mcc/event.h"
#include "mcc/shader/shader_id.h"

namespace mcc::shader {
#define FOR_EACH_SHADER_COMPILER_EVENT(V)         \
  V(ShaderCompiled)

  class ShaderCompilerEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_SHADER_COMPILER_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class ShaderCompilerEvent : public Event {
  protected:
    ShaderId id_;

    ShaderCompilerEvent(const ShaderId id):
      Event(),
      id_(id) {
    }
  public:
    ~ShaderCompilerEvent() override = default;

    ShaderId GetShaderId() const {
      return id_;
    }

#define DEFINE_TYPE_CHECK(Name)                                       \
    virtual Name##Event* As##Name##Event() { return nullptr; }        \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_SHADER_COMPILER_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_SHADER_COMPILER_EVENT(Name)                                         \
  public:                                                                           \
    std::string ToString() const override;                                          \
    const char* GetName() const override { return #Name; }                          \
    static inline Name##Event*                                                      \
    Cast(ShaderCompilerEvent* event) {                                              \
      MCC_ASSERT(event);                                                            \
      MCC_ASSERT(event->Is##Name##Event());                                         \
      return event->As##Name##Event();                                              \
    }                                                                               \
    static inline bool                                                              \
    Filter(ShaderCompilerEvent* event) {                                            \
      return event                                                                  \
          && event->As##Name##Event();                                              \
    }

  class ShaderCompiledEvent : public ShaderCompilerEvent {
  public:
    explicit ShaderCompiledEvent(const ShaderId id):
      ShaderCompilerEvent(id) {
    }
    ~ShaderCompiledEvent() override = default;
    DECLARE_SHADER_COMPILER_EVENT(ShaderCompiled);
  };
}

#endif //MCC_SHADER_COMPILER_EVENTS_H