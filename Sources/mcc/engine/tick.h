#ifndef MCC_TICK_H
#define MCC_TICK_H

#include <uv.h>
#include <vector>
#include <functional>

namespace mcc {
#define NANOSECONDS_PER_SECOND (1000000000)
#define NANOSECONDS_PER_MS (1000000)
#define TICKS_PER_SECOND 60

  typedef uint64_t TickId;

  struct Tick {
    TickId id;
    uint64_t ts;
    uint64_t dts;
  };
  
  typedef std::function<void(Tick&)> TickCallback;
  typedef std::vector<TickCallback> TickCallbackList;

  class TickListener {
  protected:
    TickListener() = default;
  public:
    virtual ~TickListener() = default;
    virtual void OnTick(Tick& tick) = 0;
  };
}

#endif //MC_TICK_H