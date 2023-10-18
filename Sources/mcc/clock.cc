#include <glog/logging.h>
#include "mcc/clock.h"

namespace mcc {
#define NS_PER_SECOND (1000000000)
#define NS_PER_MS (1000000)
#define NS_PER_TICK (NS_PER_SECOND / 60)

  void Clock::Tick() {
    const auto now = ToUnixTimestamp();
    frame_delta_ = now - last_frame_;
    last_frame_ = now;

    if((now - last_) > NS_PER_SECOND) {
      fps_ = frames_;
      tps_ = ticks_;
      frames_ = 0;
      ticks_ = 0;
      last_ = now;
      DLOG(INFO) << "FPS: " << fps_ << " TPS: " << tps_;
    }

    uint64_t tick_time = frame_delta_ + tick_remainder_;
    while(tick_time > NS_PER_TICK) {
      tick_time -= NS_PER_TICK;
    }

    tick_remainder_ = std::max(tick_time, static_cast<uint64_t>(0));
  }
}