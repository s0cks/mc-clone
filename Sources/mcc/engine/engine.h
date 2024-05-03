#ifndef MCC_ENGINE_H
#define MCC_ENGINE_H

#include <units.h>

#include "mcc/counter.h"
#include "mcc/uv/utils.h"

#include "mcc/engine/engine_state.h"
#include "mcc/engine/engine_events.h"

namespace mcc::engine {
  class Engine {
    friend class InitState;
    friend class TickState;
    friend class EngineTicker;
    friend class TerminatedState;
  protected:
    uv_loop_t* loop_;
    RelaxedAtomic<bool> running_;
    EngineState* current_state_;
    EngineState* previous_state_;
    rx::subject<EngineEvent*> events_;

    virtual void SetRunning(const bool running = true) {
      running_ = running;
    }

    inline void SetState(EngineState* state) {
      previous_state_ = current_state_;
      current_state_ = state;
    }

    template<class E, typename... Args>
    inline void Publish(Args... args) {
      E event(this, args...);
      events_.get_subscriber().on_next(&event);
    }

    template<class State>
    inline void RunState(State* state) {
      DLOG(INFO) << state->GetName() << " running.....";
      SetState(state);
      state->Run();
      const auto& duration = state->GetDurationSeries();
      using namespace units::time;
      DLOG(INFO) << state->GetName() << " done in " << nanosecond_t(duration.last()) << ", avg=" << nanosecond_t(duration.average()) << ", max=" << nanosecond_t(duration.max()) << ", min=" << nanosecond_t(duration.min());
    }

    template<class State>
    inline void RunState() {
      State state(this);
      return RunState<State>(&state);
    }

    inline rx::subject<EngineEvent*>& events() {
      return events_;
    }

    inline const rx::subject<EngineEvent*>& events() const {
      return events_;
    }
  public:
    explicit Engine(uv_loop_t* loop):
      loop_(loop),
      running_(false),
      current_state_(nullptr),
      previous_state_(nullptr),
      events_() {
    }
    virtual ~Engine() {
      if(loop_) {
        uv_loop_delete(loop_);
        loop_ = nullptr;
      }
    }

    virtual void Run();
    virtual void Shutdown();

    virtual bool IsRunning() const {
      return (bool) running_;
    }
    
    virtual uv_loop_t* GetLoop() const {
      return loop_;
    }

    virtual EngineState* GetCurrentState() const {
      return current_state_;
    }

    virtual EngineState* GetPreviousState() const {
      return previous_state_;
    }

    virtual rx::observable<EngineEvent*> OnEvent() const {
      return events_.get_observable();
    }

    const uv::TickDurationSeries& GetTickDurationSeries() const {
      MCC_ASSERT(IsRunning()); //TODO: state check
      return ((TickState*) GetCurrentState())->GetTickDurationSeries();
    }

    const uv::TicksPerSecond& GetTicksPerSecond() const {
      MCC_ASSERT(IsRunning()); //TODO: state check
      return ((TickState*) GetCurrentState())->GetTicksPerSecond();
    }

#define DEFINE_ON_ENGINE_EVENT(Name)                                   \
    rx::observable<Name##Event*> On##Name##Event() const {             \
      return OnEvent()                                                 \
        .filter([](EngineEvent* event) {                               \
          return event->Is##Name##Event();                             \
        })                                                             \
        .map([](EngineEvent* event) {                                  \
          return event->As##Name##Event();                             \
        });                                                            \
    }
  FOR_EACH_ENGINE_EVENT(DEFINE_ON_ENGINE_EVENT)
#undef DEFINE_ON_ENGINE_EVENT
  public:
    static Engine* GetEngine();
  };
}

#endif //MCC_ENGINE_H