#ifndef MCC_STATE_MACHINE_H
#define MCC_STATE_MACHINE_H

#include "mcc/common.h"
#include "mcc/relaxed_atomic.h"

namespace mcc {

#define DECLARE_STATE_MACHINE(Name)                   \
  DEFINE_NON_INSTANTIABLE_TYPE(Name);                 \
  private:                                            \
    static void SetState(const Name##State state);    \
  public:                                             \
    static Name##State GetState();

  template<typename State>
  class StateMachine {
  protected:
    RelaxedAtomic<State> state_;

    explicit StateMachine(const State& init_state):
      state_(init_state) {
    }

    void SetState(const State& state) {
      state_ = state;
    }
  public:
    virtual ~StateMachine() = default;

    State GetState() const {
      return (State) state_;
    }
  };
}

#endif //MCC_STATE_MACHINE_H