#ifndef MCC_ENGINE_STATE_H
#define MCC_ENGINE_STATE_H

#include <iostream>
#include <functional>
#include <glog/logging.h>

#include "mcc/series.h"
#include "mcc/engine/tick.h"

#include "mcc/renderer/render_timer.h"

namespace mcc::engine {
#define FOR_EACH_ENGINE_STATE(V) \
  V(Init)                        \
  V(Tick)                        \
  V(Terminated)

  class Engine;
  class State;
#define FORWARD_DECLARE_STATE(Name) class Name##State;
  FOR_EACH_ENGINE_STATE(FORWARD_DECLARE_STATE)
#undef FORWARD_DECLARE_STATE

  enum StateId : uint8_t {
    kUninitialized = 0,
#define DEFINE_STATE_ID(Name) k##Name,
    FOR_EACH_ENGINE_STATE(DEFINE_STATE_ID)
#undef DEFINE_STATE
  };

  static inline std::ostream&
  operator<<(std::ostream& stream, const StateId& rhs) {
    switch(rhs) {
      case StateId::kUninitialized: return stream << "Uninitialized";
#define DEFINE_TOSTRING(Name) case StateId::k##Name: return stream << #Name;
      FOR_EACH_ENGINE_STATE(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      default: return stream << "Unknown";
    }
  }

#define DECLARE_STATE(Name)                                             \
  protected:                                                            \
    void Apply() override;                                              \
  public:                                                               \
    StateId GetId() const override { return StateId::k##Name; }         \
    const char* GetName() const override { return #Name; }              \

  class State {
    friend class Engine;
  protected:
    Engine* engine_;
    TimeSeries<10> duration_;

    explicit State(Engine* engine):
      engine_(engine),
      duration_() {
    }

    inline Engine* engine() const {
      return engine_;
    }

    virtual void Apply() = 0;
    virtual void Shutdown() { }
  public:
    virtual ~State() = default;
    virtual StateId GetId() const = 0;
    virtual const char* GetName() const = 0;

    const TimeSeries<10> GetDuration() const {
      return duration_;
    }
  };

  class InitState : public State {
    friend class Engine;
  protected:
    explicit InitState(Engine* engine):
      State(engine) {
    }
  public:
    ~InitState() override = default;
    DECLARE_STATE(Init);
  };

  class TickState : public State {
    friend class Engine;

    template<typename T>
    static inline void
    SetState(T* handle, const TickState* state) {
      uv_handle_set_data((uv_handle_t*) handle, (void*) state);
    }

    template<typename T>
    static inline TickState*
    GetState(T* handle) {
      return (TickState*) uv_handle_get_data((uv_handle_t*) handle);
    }
  protected:
    uv_idle_t idle_;
    uv_prepare_t prepare_;
    uv_check_t check_;
    uv_async_t on_shutdown_;
    render::RenderTimer render_timer_;

    explicit TickState(Engine* engine);

    void Shutdown() override;
    static void OnIdle(uv_idle_t* handle);
    static void OnPrepare(uv_prepare_t* handle);
    static void OnCheck(uv_check_t* handle);
    static void OnShutdown(uv_async_t* handle);
    static void OnRenderTick(const render::RenderTimer::Tick& tick);
  public:
    ~TickState() override = default;
    DECLARE_STATE(Tick);
  };

  class TerminatedState : public State {
    friend class Engine;
  protected:
    explicit TerminatedState(Engine* engine):
      State(engine) {
    }
  public:
    ~TerminatedState() override = default;
    DECLARE_STATE(Terminated);
  };
}

#endif //MCC_ENGINE_STATE_H