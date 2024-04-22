#ifndef MCC_ENGINE_TICKER_H
#define MCC_ENGINE_TICKER_H

#include "mcc/uv_utils.h"

namespace mcc::engine {
  class EngineTicker {
  protected:
    uv_loop_t* loop_;
    uv_idle_t idle_;
    uv_check_t check_;
  public:
    EngineTicker() = default;
    virtual ~EngineTicker() = default;
  };
}

#endif //MCC_ENGINE_TICKER_H