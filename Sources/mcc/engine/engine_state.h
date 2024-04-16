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
  V(Terminating)                 \
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
  protected:
    Engine* engine_;

    explicit State(Engine* engine):
      engine_(engine) {
    }

    inline Engine* engine() const {
      return engine_;
    }

    virtual void Apply() = 0;
  public:
    virtual ~State() = default;
    virtual StateId GetId() const = 0;
    virtual const char* GetName() const = 0;
  };

  class PreInitState : public State {
    friend class Engine;
  protected:
    explicit PreInitState(Engine* engine):
      State(engine) {
    }
  public:
    ~PreInitState() override = default;
    DECLARE_STATE(PreInit);
  };

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

  class PostInitState : public State {
    friend class Engine;
  protected:
    explicit PostInitState(Engine* engine):
      State(engine) {
    }
  public:
    ~PostInitState() override = default;
    DECLARE_STATE(PostInit);
  };

  class TerminatingState : public State {
    friend class Engine;
  protected:
    explicit TerminatingState(Engine* engine):
      State(engine) {
    }
  public:
    ~TerminatingState() override = default;
    DECLARE_STATE(Terminating);
  };

  class TerminatedState : public State {
    friend class Engine;
  protected:
    explicit TerminatedState(Engine* engine):
      State(engine) {
    }
  public:
    ~TerminatedState() override = default;
    DECLARE_STATE(Terminated);
  };
}

#endif //MCC_ENGINE_STATE_H