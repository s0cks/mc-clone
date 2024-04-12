#ifndef MCC_ENGINE_H
#define MCC_ENGINE_H

#include "mcc/uv_utils.h"
#include "mcc/engine/engine_state.h"
#include "mcc/engine/engine_event.h"

namespace mcc::engine {
  class Engine {
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

    template<class State, class Event>
    inline void RunState(State* state) {
      DLOG(INFO) << state->GetName() << " running.....";
      const auto start = uv_hrtime();
      SetState(state);
      Publish<Event>(state, previous_state_);
      const auto total_ns = (uv_hrtime() - start);
      DLOG(INFO) << state->GetName() << " done in " << (total_ns / NSEC_PER_MSEC) << "ms.";
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