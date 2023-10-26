#include "mcc/engine/engine.h"
#include "mcc/event/event_bus.h"
#include "mcc/window.h"

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
  static RelaxedAtomic<bool> running_(false);
  static RelaxedAtomic<State> state_(kUninitialized);
  static Tick tick_;

  void Engine::Init(uv_loop_t* loop) {
    loop_ = loop;
    Engine::OnPreTick(&PreTick);
    Engine::OnPostTick(&PostTick);
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

  static std::vector<PreInitCallbackHandle*> preinit_listeners_;
  static std::vector<InitCallbackHandle*> init_listeners_;
  static std::vector<PostInitCallbackHandle*> postinit_listeners_;
  static std::vector<PreTickCallbackHandle*> pretick_listeners_;
  static std::vector<TickCallbackHandle*> tick_listeners_;
  static std::vector<PostTickCallbackHandle*> posttick_listeners_;
  static std::vector<TerminatingCallbackHandle*> terminating_listeners_;
  static std::vector<TerminatedCallbackHandle*> terminated_listeners_;

  void TickCallbackHandle::OnState() {
    callback_(tick_);
  }

  class EnginePhase {
  protected:
    uv_prepare_t handle_;

    explicit EnginePhase(uv_loop_t* loop):
      handle_() {
      uv_prepare_init(loop, &handle_);
      handle_.data = this;
      uv_prepare_start(&handle_, &OnPrepare);
    }

    static inline void
    OnPrepare(uv_prepare_t* handle) {
      const auto phase = ((EnginePhase*) handle->data);
      phase->Prepare();
    }

    virtual void Prepare() = 0;
  public:
    virtual ~EnginePhase() {
      uv_prepare_stop(&handle_);
    }

    virtual State GetState() const = 0;

    void Run(const uv_run_mode mode = UV_RUN_NOWAIT) {
      const auto state = GetState();
      LOG(INFO) << "running " << state << "....";
      const auto start = uv_hrtime();
      Engine::SetState(state);
      uv_run(loop_, mode);
      const auto total_ns = (uv_hrtime() - start);
      DLOG(INFO) << state << " done in " << (total_ns / NSEC_PER_MSEC) << "ms.";
    }
  };

  template<const State S, typename Callback>
  class EnginePhaseTemplate : public EnginePhase {
  protected:
    std::vector<Callback*>& listeners_;

    EnginePhaseTemplate(uv_loop_t* loop, std::vector<Callback*>& listeners):
      EnginePhase(loop),
      listeners_(listeners) {
    }

    void Prepare() override {
      std::for_each(listeners_.begin(), listeners_.end(), std::bind(&Callback::Send, std::placeholders::_1));
    }
  public:
    State GetState() const override {
      return S;
    }
  };

#define DEFINE_ENGINE_PHASE(Name)                                                 \
  class Name##Phase : public EnginePhaseTemplate<k##Name, Name##CallbackHandle> { \
  public:                                                                         \
    Name##Phase(uv_loop_t* loop, std::vector<Name##CallbackHandle*>& listeners):  \
      EnginePhaseTemplate<k##Name, Name##CallbackHandle>(loop, listeners) { }     \
    ~Name##Phase() override = default;                                            \
  };
  DEFINE_ENGINE_PHASE(PreInit);
  DEFINE_ENGINE_PHASE(Init);
  DEFINE_ENGINE_PHASE(PostInit);
  DEFINE_ENGINE_PHASE(PreTick);
  DEFINE_ENGINE_PHASE(PostTick);
  DEFINE_ENGINE_PHASE(Terminating);
  DEFINE_ENGINE_PHASE(Terminated);
#undef DEFINE_ENGINE_PHASE

  class TickPhase : public EnginePhase {
  protected:
    std::vector<TickCallbackHandle*>& listeners_;

    void Prepare() override {
      tick_ = Tick {
        .id = (uint64_t)total_ticks_,
        .ts = (uint64_t)ts_,
        .dts = (uint64_t)dts_,
      };
      std::for_each(listeners_.begin(), listeners_.end(), [](TickCallbackHandle* callback) {
        callback->Send();
      });
    }
  public:
    TickPhase(uv_loop_t* loop, std::vector<TickCallbackHandle*>& listeners):
      EnginePhase(loop),
      listeners_(listeners) {
    }
    ~TickPhase() override = default;

    State GetState() const override {
      return State::kTick;
    }
  };

  void Engine::PreTick() {
    ts_ = uv_hrtime();
    total_ticks_ += 1;
    ticks_ += 1;
    dts_ = (((uint64_t)ts_) - (uint64_t)last_);
    if((uint64_t) dts_ >= (NSEC_PER_MSEC * 1)) {
      tps_ = ((uint64_t) ticks_) * (1.0 * (NSEC_PER_SEC / (uint64_t) dts_));
      ticks_ = 0;
    }
  }

  void Engine::PostTick() {
    last_ = ts_;
  }

  template<typename Phase, typename Callback>
  static inline void
  RunPhase(std::vector<Callback*>& listeners) {
    Phase phase(loop_, listeners);
    phase.Run();
  }
  
  void Engine::Run() {
    RunPhase<PreInitPhase>(preinit_listeners_);
    RunPhase<InitPhase>(init_listeners_);
    RunPhase<PostInitPhase>(postinit_listeners_);
    Engine::SetRunning(true);
    while(Engine::IsRunning()) {
      RunPhase<PreTickPhase>(pretick_listeners_);
      RunPhase<TickPhase>(tick_listeners_);
      RunPhase<PostTickPhase>(posttick_listeners_);
    }
    RunPhase<TerminatingPhase>(terminating_listeners_);
    RunPhase<TerminatedPhase>(terminated_listeners_);
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

#define DEFINE_REGISTER_LISTENER(Name, Listeners) \
  void Engine::On##Name(Name##Callback callback) { \
    Listeners.push_back(new Name##CallbackHandle(loop_, callback)); \
  }

  DEFINE_REGISTER_LISTENER(PreInit, preinit_listeners_);
  DEFINE_REGISTER_LISTENER(Init, init_listeners_);
  DEFINE_REGISTER_LISTENER(PostInit, postinit_listeners_);
  DEFINE_REGISTER_LISTENER(PreTick, pretick_listeners_);
  DEFINE_REGISTER_LISTENER(Tick, tick_listeners_);
  DEFINE_REGISTER_LISTENER(PostTick, posttick_listeners_);
  DEFINE_REGISTER_LISTENER(Terminating, terminating_listeners_);
  DEFINE_REGISTER_LISTENER(Terminated, terminated_listeners_);
}