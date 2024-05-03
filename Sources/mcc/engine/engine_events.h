#ifndef MCC_ENGINE_EVENT_H
#define MCC_ENGINE_EVENT_H

#include "mcc/rx.h"
#include "mcc/event.h"
#include "mcc/uv/uv_tick.h"

namespace mcc::engine {
#define FOR_EACH_ENGINE_EVENT(V) \
  V(PreInit)                     \
  V(PostInit)                    \
  V(PreTick)                     \
  V(Tick)                        \
  V(PostTick)                    \
  V(Terminating)                 \
  V(Terminated)

  class EngineEvent;
  class StateEvent;
#define FORWARD_DECLARE_ENGINE_EVENT(Name) class Name##Event;
  FOR_EACH_ENGINE_EVENT(FORWARD_DECLARE_ENGINE_EVENT)
#undef FORWARD_DECLARE_ENGINE_EVENT

  class Engine;
  class EngineEvent : public Event {
  protected:
    Engine* engine_;

    explicit EngineEvent(Engine* engine):
      Event(),
      engine_(engine) {
    }
  public:
    virtual ~EngineEvent() = default;

    Engine* engine() const {
      return engine_;
    }

    DEFINE_EVENT_PROTOTYPE(FOR_EACH_ENGINE_EVENT);
  };

#define DECLARE_ENGINE_EVENT(Name)        \
  DECLARE_EVENT_TYPE(EngineEvent, Name)
  
  class PreInitEvent : public EngineEvent {
  public:
    explicit PreInitEvent(Engine* engine):
      EngineEvent(engine) {
    }
    ~PreInitEvent() override = default;
    DECLARE_ENGINE_EVENT(PreInit);
  };

  class PostInitEvent : public EngineEvent {
  public:
    explicit PostInitEvent(Engine* engine):
      EngineEvent(engine) {
    }
    ~PostInitEvent() override = default;
    DECLARE_ENGINE_EVENT(PostInit);
  };

  class PreTickEvent : public EngineEvent {
  public:
    explicit PreTickEvent(Engine* engine):
      EngineEvent(engine) {
    }
    ~PreTickEvent() override = default;
    DECLARE_ENGINE_EVENT(PreTick);
  };

  class TickEvent : public EngineEvent {
  protected:
    uv::Tick tick_;
  public:
    explicit TickEvent(Engine* engine, const uv::Tick& tick):
      EngineEvent(engine),
      tick_(tick) {
    }
    ~TickEvent() override = default;

    const uv::Tick& tick() const {
      return tick_;
    }

    DECLARE_ENGINE_EVENT(Tick);
  };

  class PostTickEvent : public EngineEvent {
  public:
    uv::Tick tick_;

    explicit PostTickEvent(Engine* engine, const uv::Tick& tick):
      EngineEvent(engine),
      tick_(tick) {
    }
    ~PostTickEvent() override = default;

    const uv::Tick& tick() const {
      return tick_;
    }

    DECLARE_ENGINE_EVENT(PostTick);
  };

  class TerminatingEvent : public EngineEvent {
  public:
    explicit TerminatingEvent(Engine* engine):
      EngineEvent(engine) {
    }
    ~TerminatingEvent() override = default;
    DECLARE_ENGINE_EVENT(Terminating);
  };

  class TerminatedEvent : public EngineEvent {
  public:
    explicit TerminatedEvent(Engine* engine):
      EngineEvent(engine) {
    }
    ~TerminatedEvent() override = default;
    DECLARE_ENGINE_EVENT(Terminated);
  };
}

#endif //MCC_ENGINE_EVENT_H