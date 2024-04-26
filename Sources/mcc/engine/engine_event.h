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

#define DEFINE_TYPE_CHECK(Name)                                                \
    virtual Name##Event* As##Name##Event() { return nullptr; }                 \
    virtual bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_ENGINE_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK

    friend std::ostream& operator<<(std::ostream& stream, const EngineEvent& rhs) {
      return stream << rhs.ToString();
    }
  };

#define DECLARE_ENGINE_EVENT(Name)                                                    \
  public:                                                                             \
    const char* GetName() const override { return #Name; }                            \
    Name##Event* As##Name##Event() override { return this; }                          \
    std::string ToString() const override;                                            \
    friend std::ostream& operator<<(std::ostream& stream, const Name##Event& rhs) {   \
      return stream << rhs.ToString();                                                \
    }                                                                                 \
    static inline bool                                                                \
    Filter(EngineEvent* event) {                                                      \
      return event                                                                    \
          && event->Is##Name##Event();                                                \
    }                                                                                 \
    static inline Name##Event*                                                        \
    Cast(EngineEvent* event) {                                                        \
      MCC_ASSERT(event);                                                              \
      MCC_ASSERT(event->Is##Name##Event());                                           \
      return event->As##Name##Event();                                                \
    }
  
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