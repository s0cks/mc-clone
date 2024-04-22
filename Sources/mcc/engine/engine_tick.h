#ifndef MCC_ENGINE_TICK_H
#define MCC_ENGINE_TICK_H

#include <uv.h>
#include <units.h>
#include <ostream>

#include "mcc/timestamp.h"

namespace mcc::engine {
#define NANOSECONDS_PER_SECOND (1000000000)
#define NANOSECONDS_PER_MS (1000000)

  typedef uint64_t TickId;

  struct Tick {
    TickId id;
    uint64_t ts;
    uint64_t last;
    uint64_t delta;

    Tick() = default;
    explicit Tick(const TickId tick_id,
                  const uint64_t tick_ts = uv_hrtime(),
                  const uint64_t tick_last = 0):
      id(tick_id),
      ts(tick_ts),
      last(tick_last),
      delta(tick_ts - tick_last) {
    }
    Tick(const Tick& rhs) = default;
    ~Tick() = default;

    friend std::ostream& operator<<(std::ostream& stream, const Tick& rhs) {
      using namespace units::time;
      stream << "Tick(";
      stream << "id=" << rhs.id << ", ";
      stream << "ts=" << rhs.ts << ", ";
      stream << "last=" << rhs.last << ", ";
      stream << "delta=" << nanosecond_t(rhs.delta);
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_ENGINE_TICK_H