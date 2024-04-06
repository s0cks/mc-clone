#include "mcc/engine/engine.h"
#include "mcc/engine/engine_stage_preinit.h"
#include "mcc/engine/engine_stage_init.h"
#include "mcc/engine/engine_stage_postinit.h"
#include "mcc/engine/engine_stage_tick.h"
#include "mcc/engine/engine_stage_terminating.h"
#include "mcc/engine/engine_stage_terminated.h"

#include "mcc/renderer/renderer.h"

#include "mcc/input/mouse.h"
#include "mcc/input/keyboard.h"

namespace mcc::engine {
  static uv_loop_t* loop_;
  static uint64_t ticks_;
  static uint64_t total_ticks_;
  static uint64_t ts_;
  static uint64_t dts_;
  static uint64_t tps_;
  static uint64_t last_;
  static uint64_t last_second_;
  static RelaxedAtomic<bool> running_(false);
  static RelaxedAtomic<State> state_(kUninitialized);
  static Tick current_;

  static PreInitStage preinit_;
  static InitStage init_;
  static PostInitStage postinit_;
  static TickStage tick_;
  static TerminatingStage terminating_;
  static TerminatedStage terminated_;

  static SampleSeries samples_;

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
    loop_ = uv_loop_new();
    preinit_.RunStage();
    init_.RunStage();
    postinit_.RunStage();

    SetRunning(true);
    while(IsRunning()) {
      ts_ = uv_hrtime();
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
      Mouse::Process();
      Keyboard::Process();
      tick_.RunStage();

      renderer::Renderer::Run();
      const auto duration = (uv_hrtime() - ts_);
      samples_ << Sample {
        .duration = duration,
      };
      ticks_ += 1;
      total_ticks_ += 1;
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

  SampleSeries* Engine::GetSamples() {
    return &samples_;
  }
}