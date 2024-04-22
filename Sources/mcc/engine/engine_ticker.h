#ifndef MCC_ENGINE_TICKER_H
#define MCC_ENGINE_TICKER_H

#include "mcc/rx.h"
#include "mcc/uv/uv_ticker.h"
#include "mcc/engine/engine_event.h"

namespace mcc::engine {
  class EngineTicker : public uv::Ticker {
  protected:
    rx::subject<EngineEvent*>& events_;

    void OnPreTick() override;
    void OnTick(const uv::Tick& tick) override;
    void OnPostTick(const uv::Tick& tick) override;
  public:
    EngineTicker(rx::subject<EngineEvent*>& events,
                 uv_loop_t* loop,
                 const bool start = true):
      uv::Ticker(loop, start),
      events_(events) {
    }
    virtual ~EngineTicker() = default;
  };
}

#endif //MCC_ENGINE_TICKER_H