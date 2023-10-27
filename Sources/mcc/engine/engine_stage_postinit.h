#ifndef MCC_ENGINE_STAGE_POSTINIT_H
#define MCC_ENGINE_STAGE_POSTINIT_H

#include "mcc/engine/engine_stage.h"

namespace mcc::engine {
  class PostInitStage : public Stage {
    friend class Engine;
  public:
    class PostInitCallbackHandle : public CallbackHandle<PostInitCallback> {
    private:
      static inline void
      OnSend(uv_async_t* handle) {
        ((PostInitCallbackHandle*) handle->data)->callback_();
      }
    public:
      explicit PostInitCallbackHandle(uv_loop_t* loop, PostInitCallback callback):
        CallbackHandle(loop, &OnSend, callback) {
      }
      ~PostInitCallbackHandle() override = default;
    };

    typedef std::shared_ptr<PostInitCallbackHandle> CallbackHandlePtr;
    typedef std::vector<CallbackHandlePtr> CallbackHandleList;

    static inline CallbackHandlePtr
    NewCallback(uv_loop_t* loop, PostInitCallback callback) {
      return std::make_shared<PostInitCallbackHandle>(loop, callback);
    }
  private:
    CallbackHandleList callbacks_;

    void Prepare() override {
      DLOG(INFO) << "prepare.";
      for(auto& cb : callbacks_)
        cb->Send();
    }
  public:
    explicit PostInitStage():
      Stage(),
      callbacks_() {
    }
    ~PostInitStage() override = default;

    State GetState() const override {
      return State::kPostInit;
    }

    void AddCallback(PostInitCallback callback) {
      callbacks_.push_back(NewCallback(loop_, callback));
    }
  };
}

#endif //MCC_ENGINE_STAGE_POSTINIT_H