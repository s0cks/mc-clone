#ifndef MCC_ENGINE_H
#define MCC_ENGINE_H

#include <units.h>

#include "mcc/counter.h"
#include "mcc/uv_utils.h"
#include "mcc/engine/engine_state.h"
#include "mcc/engine/engine_event.h"

namespace mcc::engine {
  class Engine {
    friend class InitState;
    friend class TickState;
    friend class TerminatedState;
  protected:
    uv_loop_t* loop_;
    RelaxedAtomic<bool> running_;
    State* current_state_;
    State* previous_state_;
    rx::subject<EngineEvent*> events_;
    uint64_t ticks_;
    uint64_t total_ticks_;
    uint64_t ts_;
    uint64_t dts_;
    uint64_t tps_;
    uint64_t last_;
    uint64_t last_second_;
    Tick current_;
    PerSecondCounter<uint64_t> tick_counter_;
    TimeSeries<10> tick_duration_;
    NumericSeries<uint64_t, 10> ticks_per_second_;

    virtual void SetRunning(const bool running = true) {
      running_ = running;
    }

    inline void SetState(State* state) {
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
      state->Apply();
      const TimeSeries<10>& duration = state->GetDuration();
      using namespace units::time;
      DLOG(INFO) << state->GetName() << " done in " << nanosecond_t(duration.last()) << ", avg=" << nanosecond_t(duration.average()) << ", max=" << nanosecond_t(duration.max()) << ", min=" << nanosecond_t(duration.min());
    }

    template<class State>
    inline void RunState() {
      State state(this);
      return RunState<State>(&state);
    }

    inline void DoPreTick(const uint64_t ts = uv_hrtime()) {
      ts_ = ts;
      dts_ = (ts_ - last_);
      if((ts_ - last_second_) >= NSEC_PER_SEC) {
        tps_ = ticks_;
        ticks_ = 0;
        last_second_ = ts_;
      }
      current_ = Tick {
        .id = total_ticks_,
        .ts = ts_,
        .dts = dts_,
      };
    }

    inline void DoPostTick(const uint64_t ts = uv_hrtime()) {
      const auto duration = (ts - ts_);
      ticks_ += 1;
      total_ticks_ += 1;
      last_ = ts_;
      tick_counter_.Increment(1, ts);
      tick_duration_.Append(duration);
      ticks_per_second_.Append(tick_counter_.per_sec());
    }
  public:
    explicit Engine(uv_loop_t* loop):
      loop_(loop),
      running_(false),
      current_state_(nullptr),
      previous_state_(nullptr),
      events_(),
      ticks_(),
      total_ticks_(),
      ts_(),
      dts_(),
      tps_(),
      last_(),
      last_second_(),
      current_() {
    }
    virtual ~Engine() {
      if(loop_) {
        uv_loop_delete(loop_);
        loop_ = nullptr;
      }
    }

    virtual void Run();
    virtual void Shutdown();

    const Tick& GetCurrentTick() const {
      return current_;
    }

    virtual bool IsRunning() const {
      return (bool) running_;
    }
    
    virtual uv_loop_t* GetLoop() const {
      return loop_;
    }

    virtual State* GetCurrentState() const {
      return current_state_;
    }

    virtual State* GetPreviousState() const {
      return previous_state_;
    }

    virtual rx::observable<EngineEvent*> OnEvent() const {
      return events_.get_observable();
    }

    uint64_t GetTotalTicks() const {
      return (uint64_t) total_ticks_;
    }

    const TimeSeries<10>& GetTickDuration() const {
      return tick_duration_;
    }

    const NumericSeries<uint64_t, 10>& GetTicksPerSecond() const {
      return ticks_per_second_;
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