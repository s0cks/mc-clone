#include "mcc/engine/engine.h"

namespace mcc::engine {
  static uv_loop_t* loop_ = nullptr;
  static uv::IdleHandle* idle_ = nullptr;
  static uv::PrepareHandle* prepare_ = nullptr;
  static uv::CheckHandle* check_ = nullptr;
  static uv_async_t* callback_ = nullptr;
  static TickCounter ticks_;
  static TickCounter total_ticks_;
  static RelaxedAtomic<uint64_t> ts_;
  static RelaxedAtomic<uint64_t> dts_;
  static RelaxedAtomic<uint64_t> tps_;
  static RelaxedAtomic<uint64_t> last_;
  static RelaxedAtomic<Engine::State> state_(Engine::kUninitialized);

  static Tick tick_;
  class EngineTickCallback {
  protected:
    uv_async_t handle_;
    TickCallback callback_;

    static inline void
    OnSend(uv_async_t* handle) {
      DLOG(INFO) << "tick: " << tick_;
      const auto& callback = ((EngineTickCallback*) handle->data)->callback_;
      callback(tick_);
    }
  public:
    EngineTickCallback(uv_loop_t* loop, TickCallback callback):
      handle_(),
      callback_(callback) {
      uv_async_init(loop, &handle_, &OnSend);
      handle_.data = this;
    }
    ~EngineTickCallback() = default;

    bool Invoke() {
      uv_async_send(&handle_);
      return true;
    }
  };

  static std::vector<EngineTickCallback*> listeners_;

  void Engine::Init(uv_loop_t* loop) {
    loop_ = loop;
    idle_ = new uv::IdleHandle(loop, &PreTick);
    prepare_ = new uv::PrepareHandle(loop, &OnTick);
    check_ = new uv::CheckHandle(loop, &PostTick);
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
    listeners_.push_back(new EngineTickCallback(loop_, callback));
  }

#define MSEC_PER_SEC (NSEC_PER_SEC / NSEC_PER_MSEC)

  void Engine::PreTick() {
    DLOG(INFO) << "pre-tick.";
    SetState(Engine::kPreTick);
    ts_ = uv_hrtime();
    total_ticks_ += 1;
    ticks_ += 1;
    dts_ = (((uint64_t)ts_) - (uint64_t)last_);
    if((uint64_t) dts_ >= (NSEC_PER_MSEC * 1)) {
      tps_ = ((uint64_t) ticks_) * (1.0 * (NSEC_PER_SEC / (uint64_t) dts_));
      ticks_ = 0;
    }
  }

  void Engine::OnTick() {
    DLOG(INFO) << "tick.";
    SetState(Engine::kTick);
    tick_ = Tick {
      .id = (uint64_t)total_ticks_,
      .ts = (uint64_t)ts_,
      .dts = (uint64_t)dts_,
    };
    std::for_each(listeners_.begin(), listeners_.end(), [](EngineTickCallback* callback) {
      callback->Invoke();
    });
  }

  void Engine::PostTick() {
    DLOG(INFO) << "post-tick.";
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