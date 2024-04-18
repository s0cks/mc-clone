#include "mcc/engine/engine.h"

#include "mcc/common.h"
#include "mcc/thread_local.h"
#include "mcc/renderer/renderer.h"

#include "mcc/keyboard/keyboard.h"

namespace mcc::engine {
  void Engine::OnIdle(uv_idle_t* handle) {
    const auto engine = GetEngine(handle);
    engine->Publish<PreTickEvent>();
    engine->DoPreTick(uv_hrtime());
  }

  void Engine::OnPrepare(uv_prepare_t* handle) {
    const auto engine = GetEngine(handle);
    engine->Publish<TickEvent>();
  }

  void Engine::OnCheck(uv_check_t* handle) {
    const auto engine = GetEngine(handle);
    engine->Publish<PostTickEvent>();
    renderer::Renderer::Run();
  }

  void Engine::OnShutdown(uv_async_t* handle) {
    const auto engine = GetEngine(handle);
    uv_idle_stop(&engine->idle_);
    uv_prepare_stop(&engine->prepare_);
    uv_check_stop(&engine->check_);
    uv_stop(engine->GetLoop());
    engine->SetRunning(false);
  }

  void Engine::Run() {
    // pre-init
    RunState<PreInitState>();
    // init
    RunState<InitState>();
    // post-init
    RunState<PostInitState>();

    SetRunning(true);
    uv_run(loop_, UV_RUN_DEFAULT);

    // terminating
    RunState<TerminatingState>();
    // terminated
    RunState<TerminatedState>();
  }

  void Engine::Shutdown() {
    uv_async_send(&on_shutdown_);
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