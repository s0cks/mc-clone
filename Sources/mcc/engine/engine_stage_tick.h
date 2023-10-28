#ifndef MCC_ENGINE_STAGE_TICK_H
#define MCC_ENGINE_STAGE_TICK_H

#include "mcc/engine/engine_stage.h"

namespace mcc::engine {
  class TickStage : public Stage {
    friend class Engine;
  public:
    class TickCallbackHandle : public CallbackHandle<std::function<void()>> {
    private:
      static inline void
      OnSend(uv_async_t* handle) {
        ((TickCallbackHandle*) handle->data)->callback_();
      }
    public:
      explicit TickCallbackHandle(uv_loop_t* loop, std::function<void()> callback):
        CallbackHandle(loop, &OnSend, callback) {
      }
      ~TickCallbackHandle() override = default;
    };

    typedef std::shared_ptr<TickCallbackHandle> CallbackHandlePtr;
    typedef std::vector<CallbackHandlePtr> CallbackHandleList;

    static inline CallbackHandlePtr
    NewCallback(uv_loop_t* loop, std::function<void()> callback) {
      return std::make_shared<TickCallbackHandle>(loop, callback);
    }
  protected:
    CallbackHandleList callbacks_;

    void Prepare() override {
      for(auto& cb : callbacks_)
        cb->Send();
    }
  public:
    explicit TickStage():
      Stage(),
      callbacks_() {  
    }
    ~TickStage() override = default;

    State GetState() const override {
      return State::kTick;
    }

    void AddCallback(std::function<void()> callback) {
      callbacks_.push_back(NewCallback(loop_, callback));
    }
  };
}

#endif //MCC_ENGINE_STAGE_TICK_H