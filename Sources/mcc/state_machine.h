#ifndef MCC_STATE_MACHINE_H
#define MCC_STATE_MACHINE_H

#include "mcc/series.h"
#include "mcc/counter.h"

namespace mcc {
  static constexpr const auto kDefaultStateSeriesCapacity = 10;
  class State {
    typedef TimeSeries<kDefaultStateSeriesCapacity> DurationSeries;
  private:
    DurationSeries duration_;
  protected:
    State() = default;

    virtual void Run() = 0;
    virtual void Stop() = 0;

    void AppendDuration(const uint64_t value) {
      duration_.Append(value);
    }
  public:
    virtual ~State() = default;
    virtual const char* GetName() const = 0;

    const DurationSeries& GetDurationSeries() const {
      return duration_;
    }
  };
}

#endif //MCC_STATE_MACHINE_H