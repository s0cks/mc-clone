#include "mcc/fixed_rate_loop.h"
#include "mcc/window.h"

#include "mcc/engine/tick.h"

namespace mcc {
  void FixedRateLoop::OnTick() {
    const auto now = uv_hrtime();
    dms_ = (now - (uint64_t)last_frame_);
    last_frame_ = now;
    if((now - (uint64_t)last_second_) > NANOSECONDS_PER_SECOND) {
      fps_ = frames_;
      tps_ = ticks_;
      frames_ = 0;
      ticks_ = 0;
      last_second_ = now;
    }
  }
}