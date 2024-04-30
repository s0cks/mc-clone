#ifndef MCC_ENGINE_STATE_H
#error "Please #include <mcc/engine/engine_state.h> instead."
#endif //MCC_ENGINE_STATE_H

#ifndef MCC_ENGINE_STATE_TERMINATED_H
#define MCC_ENGINE_STATE_TERMINATED_H

namespace mcc::engine {
  class TerminatedState : public EngineState {
    friend class Engine;
  protected:
    explicit TerminatedState(Engine* engine):
      EngineState(engine) {
    }
  public:
    ~TerminatedState() override = default;
    DECLARE_ENGINE_STATE(Terminated);
  };
}

#endif //MCC_ENGINE_STATE_TERMINATED_H