#ifndef MCC_FIXED_RATE_LOOP_H
#define MCC_FIXED_RATE_LOOP_H

#include "mcc/uv_utils.h"
#include "mcc/platform.h"
#include "mcc/timestamp.h"
#include "mcc/relaxed_atomic.h"

namespace mcc {
#define NANOSECONDS_PER_SECOND (1000000000)
#define NANOSECONDS_PER_MS (1000000)

#define TICKS_PER_SECOND 60

  namespace scene {
    class NodeUpdater;
  }

  class FixedRateLoop;
  class TickListener {
    friend class FixedRateLoop;
  protected:
    TickListener() = default;
    virtual void OnTick() = 0;
  public:
    virtual ~TickListener() = default;
  };

  class FixedRateLoop : public uv::Loop, public TickListener {
    friend class scene::NodeUpdater;
    friend class Window; //TODO: revoke
  protected:
    RelaxedAtomic<uint64_t> last_second_;
    RelaxedAtomic<uint64_t> last_frame_;
    RelaxedAtomic<uint64_t> dms_;
    RelaxedAtomic<uint64_t> total_ticks_;
    RelaxedAtomic<uint64_t> remainder_;
    RelaxedAtomic<uint64_t> frames_;
    RelaxedAtomic<uint64_t> ticks_;
    RelaxedAtomic<uint64_t> tps_;
    RelaxedAtomic<uint64_t> fps_;
    uv::IdleHandle handle_;

    explicit FixedRateLoop(uv_loop_t* loop):
      uv::Loop(loop),
      total_ticks_(0),
      dms_(0),
      last_second_(uv_hrtime()),
      last_frame_(uv_hrtime()),
      frames_(0),
      ticks_(0),
      tps_(0),
      fps_(0),
      handle_(loop, [this]() { OnTick(); }) {
    }

    void OnTick() override;
  public:
    virtual ~FixedRateLoop() = default;

    uint64_t GetTotalTicks() const {
      return (uint64_t) total_ticks_;
    }

    uint64_t GetDeltaMilliseconds() const {
      return (uint64_t) dms_;
    }

    uint64_t fps() const {
      return (uint64_t) fps_;
    }

    uint64_t tps() const {
      return (uint64_t) tps_;
    }
  };
}

#endif //MCC_FIXED_RATE_LOOP_H