#ifndef MCC_CLOCK_H
#define MCC_CLOCK_H

#include <cstdint>

#include "mcc/common.h"
#include "mcc/timestamp.h"

namespace mcc {
  class Clock {
  protected:
    uint64_t last_;
    uint64_t frames_;
    uint64_t fps_;
    uint64_t last_frame_;
    uint64_t frame_delta_;
    uint64_t ticks_;
    uint64_t tps_;
    uint64_t tick_remainder_;
  public:
    Clock() = default;
    virtual ~Clock() = default;
    DEFINE_NON_COPYABLE_TYPE(Clock);
    virtual void Tick();
  };
}

#endif //MCC_CLOCK_H