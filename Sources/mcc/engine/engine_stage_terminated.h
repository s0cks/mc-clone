#ifndef MCC_ENGINE_STATE_TERMINATED_H
#define MCC_ENGINE_STATE_TERMINATED_H

#include "mcc/engine/engine_stage.h"

namespace mcc::engine {
  class TerminatedStage : public Stage {
    friend class Engine;
  public:
    class TerminatedCallbackHandle : public CallbackHandle<TerminatedCallback> {
    private:
      static inline void
      OnSend(uv_async_t* handle) {
        ((TerminatedCallbackHandle*) handle->data)->callback_();
      }
    public:
      explicit TerminatedCallbackHandle(uv_loop_t* loop, TerminatedCallback callback):
        CallbackHandle(loop, &OnSend, callback) {
      }
      ~TerminatedCallbackHandle() override = default;
    };

    typedef std::shared_ptr<TerminatedCallbackHandle> CallbackHandlePtr;
    typedef std::vector<CallbackHandlePtr> CallbackHandleList;

    static inline CallbackHandlePtr
    NewCallback(uv_loop_t* loop, TerminatedCallback callback) {
      return std::make_shared<TerminatedCallbackHandle>(loop, callback);
    }
  private:
    CallbackHandleList callbacks_;

    void Prepare() override {
      DLOG(INFO) << "prepare.";
      for(auto& cb : callbacks_)
        cb->Send();
    }
  public:
    explicit TerminatedStage():
      Stage(),
      callbacks_() {
    }
    ~TerminatedStage() override = default;

    State GetState() const override {
      return State::kTerminated;
    }

    void AddCallback(TerminatedCallback callback) {
      callbacks_.push_back(NewCallback(loop_, callback));
    }
  };
}

#endif //MCC_ENGINE_STATE_TERMINATED_H