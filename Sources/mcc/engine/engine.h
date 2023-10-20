#ifndef MCC_ENGINE_H
#define MCC_ENGINE_H

#include "mcc/uv_utils.h"
#include "mcc/engine/tick.h"
#include "mcc/relaxed_atomic.h"

namespace mcc {
  typedef RelaxedAtomic<uint64_t> TickCounter;

  class Engine : public uv::Loop {
  protected:
    uv::IdleHandle pre_;
    uv::PrepareHandle tick_;
    uv::CheckHandle post_;

    TickCounter total_ticks_;
    TickCounter ticks_;
    RelaxedAtomic<uint64_t> ts_;
    RelaxedAtomic<uint64_t> last_;
    TickCallbackList listeners_;

    void PreTick() {
      ts_ = uv_hrtime();
    }

    void OnTick() {
      const auto dts = (((uint64_t)ts_) - (uint64_t)last_);
      Tick tick = {
        .id = (uint64_t)total_ticks_,
        .ts = (uint64_t)ts_,
        .dts = dts,
      };
      std::for_each(listeners_.begin(), listeners_.end(), [&tick](const TickCallback& listener) {
        listener(tick);
      });
    }

    void PostTick() {
      total_ticks_ += 1;
      last_ = ts_;
    }
  public:
    explicit Engine(uv_loop_t* loop):
      uv::Loop(loop),
      pre_(loop, [this]() { PreTick(); }),
      post_(loop, [this]() { PostTick(); }),
      tick_(loop, [this]() { OnTick(); }),
      listeners_() {
    }
    ~Engine() override = default;

    void Register(TickCallback callback) {
      listeners_.push_back(callback);
    }

    void Register(TickListener* listener) {
      return Register(std::bind(&TickListener::OnTick, listener, std::placeholders::_1));
    }
  };
}

#endif //MCC_ENGINE_H