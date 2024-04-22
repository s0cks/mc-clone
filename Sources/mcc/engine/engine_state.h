#ifndef MCC_ENGINE_STATE_H
#define MCC_ENGINE_STATE_H

#include <iostream>
#include <functional>
#include <glog/logging.h>

#include "mcc/series.h"
#include "mcc/engine/tick.h"
#include "mcc/engine/engine_ticker.h"

namespace mcc::engine {
#define FOR_EACH_ENGINE_STATE(V) \
  V(Init)                        \
  V(Tick)                        \
  V(Terminated)

  class Engine;
  class State;
#define FORWARD_DECLARE_STATE(Name) class Name##State;
  FOR_EACH_ENGINE_STATE(FORWARD_DECLARE_STATE)
#undef FORWARD_DECLARE_STATE

  enum StateId : uint8_t {
    kUninitialized = 0,
#define DEFINE_STATE_ID(Name) k##Name,
    FOR_EACH_ENGINE_STATE(DEFINE_STATE_ID)
#undef DEFINE_STATE
  };

  static inline std::ostream&
  operator<<(std::ostream& stream, const StateId& rhs) {
    switch(rhs) {
      case StateId::kUninitialized: return stream << "Uninitialized";
#define DEFINE_TOSTRING(Name) case StateId::k##Name: return stream << #Name;
      FOR_EACH_ENGINE_STATE(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      default: return stream << "Unknown";
    }
  }

#define DECLARE_STATE(Name)                                             \
  protected:                                                            \
    void Apply() override;                                              \
  public:                                                               \
    StateId GetId() const override { return StateId::k##Name; }         \
    const char* GetName() const override { return #Name; }              \

  class State {
    friend class Engine;
  protected:
    Engine* engine_;
    TimeSeries<10> duration_;

    explicit State(Engine* engine):
      engine_(engine),
      duration_() {
    }

    inline Engine* engine() const {
      return engine_;
    }

    virtual void Apply() = 0;
    virtual void Shutdown() { }
  public:
    virtual ~State() = default;
    virtual StateId GetId() const = 0;
    virtual const char* GetName() const = 0;

    const TimeSeries<10> GetDuration() const {
      return duration_;
    }
  };
}

#include "mcc/engine/engine_state_init.h"
#include "mcc/engine/engine_state_tick.h"
#include "mcc/engine/engine_state_terminated.h"

#endif //MCC_ENGINE_STATE_H