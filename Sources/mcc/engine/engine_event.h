#ifndef MCC_ENGINE_EVENT_H
#define MCC_ENGINE_EVENT_H

#include "mcc/rx.h"
#include "mcc/engine/engine_state.h"

namespace mcc::engine {
#define FOR_EACH_ENGINE_EVENT(V) \
  V(State)                       \
  FOR_EACH_ENGINE_STATE(V)       \
  V(PreTick)                     \
  V(Tick)                        \
  V(PostTick)

#define FORWARD_DECLARE_ENGINE_EVENT(Name) class Name##Event;
  FOR_EACH_ENGINE_EVENT(FORWARD_DECLARE_ENGINE_EVENT)
#undef FORWARD_DECLARE_ENGINE_EVENT

  class Engine;
  class EngineEvent {
  protected:
    Engine* engine_;

    explicit EngineEvent(Engine* engine):
      engine_(engine) {
    }
  public:
    virtual ~EngineEvent() = default;
    virtual const char* GetName() const = 0;

    Engine* engine() const {
      return engine_;
    }

#define DEFINE_TYPE_CHECK(Name)                                                \
    virtual Name##Event* As##Name##Event() { return nullptr; }         \
    virtual bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_ENGINE_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_ENGINE_EVENT(Name)                                    \
  public:                                                             \
    const char* GetName() const override { return #Name; }            \
    Name##Event* As##Name##Event() override { return this; }

  class StateEvent : public EngineEvent {
  protected:
    explicit StateEvent(Engine* engine):
      EngineEvent(engine) {
    }
  public:
    ~StateEvent() override = default;

    StateEvent* AsStateEvent() override {
      return this;
    }
  };

#define DEFINE_STATE_EVENT(Name)              \
  class Name##Event : public StateEvent {     \
  public:                                     \
    explicit Name##Event(Engine* engine):     \
      StateEvent(engine) {                    \
    }                                         \
    ~Name##Event() override = default;        \
    DECLARE_ENGINE_EVENT(Name);               \
  };

  FOR_EACH_ENGINE_STATE(DEFINE_STATE_EVENT);
#undef DEFINE_STATE_EVENT

  class PreTickEvent : public EngineEvent {
  public:
    explicit PreTickEvent(Engine* engine):
      EngineEvent(engine) {
    }
    ~PreTickEvent() override = default;
    DECLARE_ENGINE_EVENT(PreTick);
  };

  class TickEvent : public EngineEvent {
  public:
    explicit TickEvent(Engine* engine):
      EngineEvent(engine) {
    }
    ~TickEvent() override = default;
    DECLARE_ENGINE_EVENT(Tick);
  };

  class PostTickEvent : public EngineEvent {
  public:
    explicit PostTickEvent(Engine* engine):
      EngineEvent(engine) {
    }
    ~PostTickEvent() override = default;
    DECLARE_ENGINE_EVENT(PostTick);
  };
}

#endif //MCC_ENGINE_EVENT_H