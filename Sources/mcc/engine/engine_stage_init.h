#ifndef MCC_ENGINE_STAGE_INIT_H
#define MCC_ENGINE_STAGE_INIT_H

#include <memory>
#include "mcc/engine/engine_stage.h"

namespace mcc::engine {
  class InitStage : public Stage {
    friend class Engine;
  public:
    class InitCallbackHandle : public CallbackHandle<InitCallback> {
    private:
      static inline void
      OnSend(uv_async_t* handle) {
        ((InitCallbackHandle*) handle->data)->callback_();
      }
    public:
      explicit InitCallbackHandle(uv_loop_t* loop, InitCallback callback):
        CallbackHandle(loop, &OnSend, callback) {
      }
      ~InitCallbackHandle() override = default;
    };

    typedef std::shared_ptr<InitCallbackHandle> CallbackHandlePtr;
    typedef std::vector<CallbackHandlePtr> CallbackHandleList;

    static inline CallbackHandlePtr
    NewCallback(uv_loop_t* loop, InitCallback callback) {
      return std::make_shared<InitCallbackHandle>(loop, callback);
    }
  protected:
    CallbackHandleList callbacks_;

    void Prepare() override {
      DLOG(INFO) << "prepare.";
      for(auto& cb : callbacks_)
        cb->Send();
    }
  public:
    explicit InitStage():
      Stage(),
      callbacks_() {
    }
    ~InitStage() override = default;
    
    State GetState() const override {
      return State::kInit;
    }

    void AddCallback(InitCallback callback) {
      callbacks_.push_back(NewCallback(loop_, callback));
    }
  };
}

#endif //MCC_ENGINE_STAGE_INIT_H