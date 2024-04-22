#ifndef MCC_UV_TICKER_H
#define MCC_UV_TICKER_H

#include <units.h>
#include "mcc/series.h"
#include "mcc/counter.h"

#include "mcc/uv/uv_tick.h"
#include "mcc/uv/uv_handle.h"

namespace mcc::uv {
  typedef TimeSeries<10> TickDurationSeries;
  typedef PerSecondCounter<uint64_t> TicksPerSecond;

  class Ticker : public uv::IdleListener,
                 public uv::PrepareListener,
                 public uv::CheckListener {
  protected:
    uv_loop_t* loop_;
    uv::IdleHandle idle_;
    uv::PrepareHandle prepare_;
    uv::CheckHandle check_;
    TicksPerSecond count_;
    uint64_t current_;
    uint64_t last_;
    TickDurationSeries duration_;

    virtual void OnPreTick() = 0;
    virtual void OnTick(const Tick& tick) = 0;
    virtual void OnPostTick(const Tick& tick) = 0;

    void OnIdle() override {
      return OnPreTick();
    }

    void OnPrepare() override {
      last_ = current_;
      current_ = uv::Now();
      const auto tick = Tick((uint64_t) count_, current_, last_);
      count_.Increment(1, current_);
      return OnTick(tick);
    }

    void OnCheck() override {
      const auto tick = Tick(((uint64_t) count_) - 1, current_, last_);
      duration_.Append(tick.delta);
      return OnPostTick(tick);
    }
  public:
    explicit Ticker(uv_loop_t* loop, const bool start = true):
      loop_(loop),
      idle_(loop, this, start),
      prepare_(loop, this, start),
      check_(loop, this, start),
      count_(0),
      current_(0),
      last_(0),
      duration_() {
    }
    ~Ticker() override {
      Stop();
    }

    uint64_t GetTotalTicks() const {
      return (uint64_t) count_;
    }

    const TicksPerSecond& GetTicksPerSecond() const {
      return count_;
    }

    const TickDurationSeries& GetTickDurationSeries() const {
      return duration_;
    }

    uint64_t GetCurrentTickNanoseconds() const {
      return current_;
    }

    uint64_t GetLastTickNanoseconds() const {
      return last_;
    }
    
    void Stop() {
      idle_.Stop();
      prepare_.Stop();
      check_.Stop();
    }
  };
}

#endif //MCC_UV_TICKER_H