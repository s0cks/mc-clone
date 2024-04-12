#ifndef MCC_ENGINE_TICK_H
#define MCC_ENGINE_TICK_H

#include <iostream>

namespace mcc::engine {
#define NANOSECONDS_PER_SECOND (1000000000)
#define NANOSECONDS_PER_MS (1000000)

  typedef uint64_t TickId;

  struct Tick {
    TickId id;
    uint64_t ts;
    uint64_t dts;

    friend std::ostream& operator<<(std::ostream& stream, const Tick& rhs) {
      stream << "Tick(";
      stream << "id=" << rhs.id << ", ";
      stream << "ts=" << rhs.ts << ", ";
      stream << "dts=" << rhs.dts;
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_ENGINE_TICK_H