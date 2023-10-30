#include "mcc/engine/engine.h"
#include "mcc/event/event_bus.h"
#include "mcc/window.h"

#include "mcc/engine/engine_stage_preinit.h"
#include "mcc/engine/engine_stage_init.h"
#include "mcc/engine/engine_stage_postinit.h"
#include "mcc/engine/engine_stage_tick.h"
#include "mcc/engine/engine_stage_terminating.h"
#include "mcc/engine/engine_stage_terminated.h"

namespace mcc::engine {
  static uv_loop_t* loop_ = nullptr;
  static TickCounter ticks_;
  static TickCounter total_ticks_;
  static RelaxedAtomic<uint64_t> ts_;
  static RelaxedAtomic<uint64_t> dts_;
  static RelaxedAtomic<uint64_t> tps_;
  static RelaxedAtomic<uint64_t> last_;
  static RelaxedAtomic<bool> running_(false);
  static RelaxedAtomic<State> state_(kUninitialized);
  static Tick current_;

  static PreInitStage preinit_;
  static InitStage init_;
  static PostInitStage postinit_;
  static TickStage tick_;
  static TerminatingStage terminating_;
  static TerminatedStage terminated_;

  Tick Engine::GetTick() {
    return current_;
  }

  void Engine::Init(uv_loop_t* loop) {
    loop_ = loop;
  }

  void Engine::SetState(const State value) {
    state_ = value;
  }

  State Engine::GetState() {
    return (State) state_;
  }

  uv_loop_t* Engine::GetLoop() {
    return loop_;
  }

#define MSEC_PER_SEC (NSEC_PER_SEC / NSEC_PER_MSEC)

  uint64_t Engine::GetTotalTicks() {
    return (uint64_t) total_ticks_;
  }

  uint64_t Engine::GetTPS() {
    return (uint64_t) tps_;
  }

  template<class Stage, typename CallbackList>
  static inline void RunStage(CallbackList& callbacks) {
    Stage stage(callbacks);
    stage.RunStage();
  }
  
  void Engine::Run() {
    preinit_.RunStage();
    init_.RunStage();
    postinit_.RunStage();
    SetRunning(true);
    while(IsRunning()) {
      ts_ = uv_hrtime();
      total_ticks_ += 1;
      ticks_ += 1;
      dts_ = (((uint64_t)ts_) - (uint64_t)last_);
      if((uint64_t) dts_ >= (NSEC_PER_MSEC * 1)) {
        tps_ = ((uint64_t) ticks_) * (1.0 * (NSEC_PER_SEC / (uint64_t) dts_));
        ticks_ = 0;
      }
      current_ = Tick {
        .id = (uint64_t) total_ticks_, 
        .ts = (uint64_t) ts_,
        .dts = (uint64_t) dts_,
      };
      tick_.RunStage();
      last_ = ts_;
    }
    
    terminating_.RunStage();
    terminated_.RunStage();
  }

  void Engine::Shutdown() {
    return Engine::SetRunning(false);
  }

  void Engine::SetRunning(const bool value) {
    running_ = value;
  }

  bool Engine::IsRunning() {
    return (bool) running_;
  }

  void Engine::OnPreInit(PreInitCallback callback) {
    return preinit_.AddCallback(callback);
  }
  
  void Engine::OnInit(InitCallback callback) {
    return init_.AddCallback(callback);
  }

  void Engine::OnPostInit(PostInitCallback callback) {
    return postinit_.AddCallback(callback);
  }

  void Engine::OnTick(TickCallback callback) {
    const auto fn = [callback]() {
      callback(current_);
    };
    return tick_.AddCallback(fn);
  }

  void Engine::OnTerminating(TerminatingCallback callback) {
    return terminating_.AddCallback(callback);
  }

  void Engine::OnTerminated(TerminatedCallback callback) {
    return terminated_.AddCallback(callback);
  }
}