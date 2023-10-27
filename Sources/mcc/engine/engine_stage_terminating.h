#ifndef MCC_ENGINE_STAGE_TERMINATING_H
#define MCC_ENGINE_STAGE_TERMINATING_H

#include "mcc/engine/engine_stage.h"

namespace mcc::engine {
  class TerminatingStage : public Stage {
    friend class Engine;
  public:
    class TerminatingCallbackHandle : public CallbackHandle<TerminatingCallback> {
    private:
      static inline void
      OnSend(uv_async_t* handle) {
        ((TerminatingCallbackHandle*) handle->data)->callback_();
      }
    public:
      explicit TerminatingCallbackHandle(uv_loop_t* loop, TerminatingCallback callback):
        CallbackHandle(loop, &OnSend, callback) {
      }
      ~TerminatingCallbackHandle() override = default;
    };

    typedef std::shared_ptr<TerminatingCallbackHandle> CallbackHandlePtr;
    typedef std::vector<CallbackHandlePtr> CallbackHandleList;

    static inline CallbackHandlePtr
    NewCallback(uv_loop_t* loop, TerminatingCallback callback) {
      return std::make_shared<TerminatingCallbackHandle>(loop, callback);
    }
  private:
    CallbackHandleList callbacks_;

    void Prepare() override {
      DLOG(INFO) << "prepare.";
      for(auto& cb : callbacks_)
        cb->Send();
    }
  public:
    explicit TerminatingStage():
      Stage(),
      callbacks_() {
    }
    ~TerminatingStage() override = default;

    State GetState() const override {
      return State::kTerminating;
    }

    void AddCallback(TerminatingCallback callback) {
      callbacks_.push_back(NewCallback(loop_, callback));
    }
  };
}

#endif //MCC_ENGINE_STAGE_TERMINATING_H