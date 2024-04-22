#ifndef MCC_ENGINE_STATE_H
#error "Please #include <mcc/engine/engine_state.h> instead."
#endif //MCC_ENGINE_STATE_H

#ifndef MCC_ENGINE_STATE_INIT_H
#define MCC_ENGINE_STATE_INIT_H

namespace mcc::engine {
  class InitState : public State {
    friend class Engine;
  protected:
    explicit InitState(Engine* engine):
      State(engine) {
    }
  public:
    ~InitState() override = default;
    DECLARE_STATE(Init);
  };
}

#endif //MCC_ENGINE_STATE_INIT_H