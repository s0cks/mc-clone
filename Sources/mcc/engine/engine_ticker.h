#ifndef MCC_ENGINE_TICKER_H
#define MCC_ENGINE_TICKER_H

#include "mcc/rx.h"
#include "mcc/series.h"
#include "mcc/counter.h"
#include "mcc/uv/uv_handle.h"

#include "mcc/engine/engine_tick.h"
#include "mcc/engine/engine_event.h"

namespace mcc::engine {
  class EngineTicker : public uv::IdleListener,
                       public uv::PrepareListener,
                       public uv::CheckListener {
  public:
    typedef TimeSeries<10> DurationSeries;
    typedef PerSecondCounter<uint64_t> TicksPerSecond;
  protected:
    rx::subject<EngineEvent*>& events_;
    uv_loop_t* loop_;
    uv::IdleHandle idle_;
    uv::PrepareHandle prepare_;
    uv::CheckHandle check_;
    TicksPerSecond count_;
    uint64_t current_;
    uint64_t last_;
    DurationSeries duration_;

    void OnIdle() override;
    void OnPrepare() override;
    void OnCheck() override;
  public:
    explicit EngineTicker(rx::subject<EngineEvent*>& events,
                          uv_loop_t* loop):
      events_(events),
      loop_(loop),
      idle_(loop, this),
      prepare_(loop, this),
      check_(loop, this),
      count_(0),
      current_(0),
      last_(0) {
    }
    virtual ~EngineTicker() {
      Stop();
    }

    uint64_t GetTotalTicks() const {
      return count_;
    }

    uint64_t GetCurrentTickNanoseconds() const {
      return current_;
    }

    uint64_t GetLastTickNanoseconds() const {
      return last_;
    }

    const DurationSeries& GetDurationSeries() const {
      return duration_;
    }

    const TicksPerSecond& GetTicksPerSecond() const {
      return count_;
    }

    void Stop() {
      idle_.Stop();
      prepare_.Stop();
      check_.Stop();
      uv_stop(loop_);
    }
  };
}

#endif //MCC_ENGINE_TICKER_H