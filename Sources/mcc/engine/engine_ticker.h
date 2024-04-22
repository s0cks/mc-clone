#ifndef MCC_ENGINE_TICKER_H
#define MCC_ENGINE_TICKER_H

#include "mcc/uv/utils.h"

namespace mcc::engine {
  class EngineTicker : public uv::IdleListener {
  protected:
    uv_loop_t* loop_;
    uv::IdleHandle idle_;

    void OnIdle() override {
      DLOG(INFO) << "idle.";
    }
  public:
    explicit EngineTicker(uv_loop_t* loop):
      loop_(loop),
      idle_(loop, this) {
    }
    virtual ~EngineTicker() {
      Stop();
    }

    void Stop() {
      idle_.Stop();
    }
  };
}

#endif //MCC_ENGINE_TICKER_H