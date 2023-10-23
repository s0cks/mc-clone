#include "mcc/engine/engine.h"

namespace mcc::engine {
  static uv_loop_t* loop_ = nullptr;
  static uv::IdleHandle* pre_ = nullptr;
  static uv::CheckHandle* post_ = nullptr;
  static TickCallbackList listeners_;
  static TickCounter ticks_;
  static TickCounter total_ticks_;
  static RelaxedAtomic<uint64_t> ts_;
  static RelaxedAtomic<uint64_t> tps_;
  static RelaxedAtomic<uint64_t> last_;
  static RelaxedAtomic<Engine::State> state_(Engine::kUninitialized);

  void Engine::Init(uv_loop_t* loop) {
    loop_ = loop;
    pre_ = new uv::IdleHandle(loop, &PreTick);
    post_ = new uv::CheckHandle(loop, &PostTick);
    SetState(Engine::kInitialized);
  }

  void Engine::SetState(const Engine::State value) {
    state_ = value;
  }

  Engine::State Engine::GetState() {
    return (Engine::State) state_;
  }

  uv_loop_t* Engine::GetLoop() {
    return loop_;
  }

  void Engine::Register(TickCallback callback) {
    listeners_.push_back(callback);
  }

#define MSEC_PER_SEC (NSEC_PER_SEC / NSEC_PER_MSEC)

  void Engine::PreTick() {
    SetState(Engine::kPreTick);
    ts_ = uv_hrtime();
    total_ticks_ += 1;
    ticks_ += 1;
    const auto dts = (((uint64_t)ts_) - (uint64_t)last_);
    if(dts >= (NSEC_PER_MSEC * 1)) {
      tps_ = ((uint64_t) ticks_) * (1.0 * (NSEC_PER_SEC / dts));
      ticks_ = 0;
    }

    Tick tick = {
      .id = (uint64_t)total_ticks_,
      .ts = (uint64_t)ts_,
      .dts = dts,
    };
    std::for_each(listeners_.begin(), listeners_.end(), [&tick](const TickCallback& listener) {
      listener(tick);
    });
    SetState(Engine::kTick);
  }

  void Engine::PostTick() {
    SetState(Engine::kPostTick);
    last_ = ts_;
  } 

  uint64_t Engine::GetTotalTicks() {
    return (uint64_t) total_ticks_;
  }

  uint64_t Engine::GetTPS() {
    return (uint64_t) tps_;
  }
}