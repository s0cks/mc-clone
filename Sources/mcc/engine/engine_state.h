#ifndef MCC_ENGINE_STATE_H
#define MCC_ENGINE_STATE_H

#include <iostream>
#include <functional>
#include <glog/logging.h>
#include "mcc/engine/tick.h"

namespace mcc::engine {
#define FOR_EACH_ENGINE_STATE(V) \
  V(PreInit)                     \
  V(Init)                        \
  V(PostInit)                    \
  V(Tick)                        \
  V(Terminating)                 \
  V(Terminated)

  enum State {
    kUninitialized,
#define DEFINE_STATE(Name) k##Name,
    FOR_EACH_ENGINE_STATE(DEFINE_STATE)
#undef DEFINE_STATE
  };

  static inline std::ostream&
  operator<<(std::ostream& stream, const State& rhs) {
    switch(rhs) {
      case kUninitialized: return stream << "Uninitialized";
#define DEFINE_TOSTRING(Name) case State::k##Name: return stream << #Name;
      FOR_EACH_ENGINE_STATE(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      default: return stream << "Unknown";
    }
  }

  typedef std::function<void()> PreInitCallback;
  typedef std::function<void()> InitCallback;
  typedef std::function<void()> PostInitCallback;
  typedef std::function<void(const Tick&)> TickCalback;
  typedef std::function<void()> TerminatingCallback;
  typedef std::function<void()> TerminatedCallback;
}

#endif //MCC_ENGINE_STATE_H