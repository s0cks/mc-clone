#ifndef MCC_TICK_H
#define MCC_TICK_H

#include <uv.h>
#include <vector>
#include <iostream>
#include <functional>

namespace mcc {
  namespace engine {
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

    typedef std::function<void(Tick&)> TickCallback;

    class TickListener {
    protected:
      TickListener() = default;
    public:
      virtual ~TickListener() = default;
      virtual void OnTick(Tick& tick) = 0;
    };
  }
  using engine::Tick;
  using engine::TickListener;
}

#endif //MC_TICK_H