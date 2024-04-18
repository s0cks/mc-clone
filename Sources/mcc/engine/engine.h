#ifndef MCC_ENGINE_H
#define MCC_ENGINE_H

#include "mcc/uv_utils.h"
#include "mcc/engine/engine_state.h"
#include "mcc/engine/engine_event.h"

namespace mcc::engine {
  class Engine {
    friend class PreInitState;
    friend class InitState;
    friend class PostInitState;
    friend class TerminatingState;
    friend class TerminatedState;
  protected:
    uv_loop_t* loop_;
    uv_idle_t idle_;
    uv_prepare_t prepare_;
    uv_check_t check_;
    uv_async_t on_shutdown_;

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

    template<class State>
    inline void RunState(State* state) {
      DLOG(INFO) << state->GetName() << " running.....";
      const auto start = uv_hrtime();
      SetState(state);
      state->Apply();
      const auto total_ns = (uv_hrtime() - start);
      DLOG(INFO) << state->GetName() << " done in " << (total_ns / NSEC_PER_MSEC) << "ms.";
    }

    template<class State>
    inline void RunState() {
      State state(this);
      return RunState<State>(&state);
    }

    inline void DoPreTick(const uint64_t ts) {
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

    inline void DoPostTick(const uint64_t ts) {
      const auto duration = (ts - ts_);
      ticks_ += 1;
      total_ticks_ += 1;
      last_ = ts_;
    }
  private:
    template<typename H>
    static inline Engine*
    GetEngine(H* handle) {
      return (Engine*) uv_handle_get_data((uv_handle_t*) handle);
    }

    template<typename H>
    static inline void
    SetEngine(H* handle, Engine* engine) {
      uv_handle_set_data((uv_handle_t*) handle, (void*) engine);
    }

    static void OnIdle(uv_idle_t* handle);
    static void OnPrepare(uv_prepare_t* handle);
    static void OnCheck(uv_check_t* handle);
    static void OnShutdown(uv_async_t* handle);
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
      current_(),
      idle_(),
      prepare_(),
      check_(),
      on_shutdown_() {
      SetEngine(&idle_, this);
      uv_idle_init(loop, &idle_);
      uv_idle_start(&idle_, &OnIdle);

      SetEngine(&prepare_, this);
      uv_prepare_init(loop, &prepare_);
      uv_prepare_start(&prepare_, &OnPrepare);

      SetEngine(&check_, this);
      uv_check_init(loop, &check_);
      uv_check_start(&check_, &OnCheck);

      SetEngine(&on_shutdown_, this);
      uv_async_init(loop, &on_shutdown_, &OnShutdown);
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