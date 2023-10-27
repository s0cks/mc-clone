#ifndef MCC_ENGINE_STAGE_PREINIT_H
#define MCC_ENGINE_STAGE_PREINIT_H

#include <memory>
#include "mcc/engine/engine_stage.h"

namespace mcc::engine {
  class PreInitStage : public Stage {
    friend class Engine;
  public:
    class PreInitCallbackHandle : public CallbackHandle<PreInitCallback> {
    private:
      static inline void
      OnSend(uv_async_t* handle) {
        ((PreInitCallbackHandle*) handle->data)->callback_();
      }
    public:
      explicit PreInitCallbackHandle(uv_loop_t* loop, PreInitCallback callback):
        CallbackHandle(loop, &OnSend, callback) {
      }
      ~PreInitCallbackHandle() override = default;
    };

    typedef std::shared_ptr<PreInitCallbackHandle> CallbackHandlePtr;
    typedef std::vector<CallbackHandlePtr> CallbackHandleList;

    static inline CallbackHandlePtr
    NewCallback(uv_loop_t* loop, PreInitCallback callback) {
      return std::make_shared<PreInitCallbackHandle>(loop, callback);
    }
  protected:
    CallbackHandleList callbacks_;

    void Prepare() override {
      DLOG(INFO) << "prepare.";
      for(auto& cb : callbacks_)
        cb->Send();
    }
  public:
    explicit PreInitStage():
      Stage(),
      callbacks_() {  
    }
    ~PreInitStage() override = default;

    State GetState() const override {
      return State::kPreInit;
    }

    void AddCallback(PreInitCallback callback) {
      callbacks_.push_back(NewCallback(loop_, callback));
    }
  };
}

#endif //MCC_ENGINE_STAGE_PREINIT_H