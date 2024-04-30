#ifndef MCC_RENDER_TICKER_H
#define MCC_RENDER_TICKER_H

#include "mcc/uv/uv_ticker.h"

namespace mcc::render {
  class RenderTicker : public uv::RateLimitedTicker {
  public:
    static constexpr const uint64_t kDefaultRate = NSEC_PER_SEC / 30;
  protected:
    void OnPreTick() override;
    void OnTick(const uv::Tick& tick) override;
    void OnPostTick(const uv::Tick& tick) override;
  public:
    RenderTicker(uv_loop_t* loop,
                 const uint64_t rate = kDefaultRate,
                 const bool start = true):
      uv::RateLimitedTicker(loop, rate, start) {
    }
    virtual ~RenderTicker() = default;
  };
}

#endif //MCC_RENDER_TICKER_H