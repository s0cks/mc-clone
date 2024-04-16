#include "mcc/engine/engine.h"

#include "mcc/common.h"
#include "mcc/thread_local.h"
#include "mcc/renderer/renderer.h"

namespace mcc::engine {
  void Engine::Run() {
    {
      // pre-init
      const auto state = new PreInitState();
      RunState<PreInitState, PreInitEvent>(state);
      delete state;
    }

    {
      // init
      const auto state = new InitState();
      RunState<InitState, InitEvent>(state);
      delete state;
    }

    {
      // post-init
      const auto state = new PostInitState();
      RunState<PostInitState, PostInitEvent>(state);
      delete state;
    }

    SetRunning(true);
    while(IsRunning()) {
      Publish<PreTickEvent>();

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

      const auto mouse = GetMouse();
      if(mouse)
        mouse->Process();

      Publish<TickEvent>();

      const auto duration = (uv_hrtime() - ts_);
      ticks_ += 1;
      total_ticks_ += 1;
      last_ = ts_;

      Publish<PostTickEvent>();

      renderer::Renderer::Run();
    }

    {
      // terminating
      const auto state = new TerminatingState();
      RunState<TerminatingState, TerminatingEvent>(state);
      delete state;
    }

    {
      // terminated
      const auto state = new TerminatedState();
      RunState<TerminatedState, TerminatedEvent>(state);
      delete state;
    }
  }

  void Engine::Shutdown() {
    running_ = false;
  }

  static ThreadLocal<Engine> engine_;

  Engine* Engine::GetEngine() {
    if(engine_)
      return engine_.Get();
    const auto engine = new Engine(uv_loop_new());
    engine_.Set(engine);
    return engine;
  }
}