#ifndef MCC_ENGINE_STAGE_H
#define MCC_ENGINE_STAGE_H

#include "mcc/engine/engine_state.h"

namespace mcc::engine {
  class Stage {
  protected:
    template<typename Callback>
    class CallbackHandle {
    protected:
      uv_async_t handle_;
      Callback callback_;
    
      explicit CallbackHandle(uv_loop_t* loop, uv_async_cb on_send, Callback callback):
        handle_(),
        callback_(callback) {
        uv_async_init(loop, &handle_, on_send);
        handle_.data = this;
      }
    public:
      virtual ~CallbackHandle() = default;

      void Send() {
        uv_async_send(&handle_);
      }
    };
  private:
    static inline void
    OnPrepare(uv_prepare_t* handle) {
      ((Stage*) handle->data)->Prepare();
    }

    static inline void
    OnCheck(uv_check_t* handle) {
      ((Stage*) handle->data)->Check();
    }
  protected:
    uv_loop_t* loop_;
    uv_prepare_t prepare_;
    uv_check_t check_;

    Stage():
      loop_(uv_loop_new()),
      prepare_(),
      check_() {
      loop_->data = this;
      uv_prepare_init(loop_, &prepare_);
      uv_prepare_start(&prepare_, &OnPrepare);
      prepare_.data = this;
      uv_check_init(loop_, &check_);
      uv_check_start(&check_, &OnCheck);
      check_.data = this;
    }

    virtual void Prepare() = 0;
    
    virtual void Check() {
      uv_stop(loop_);
    }
  public:
    virtual ~Stage() {
      uv_check_stop(&check_);
      uv_close((uv_handle_t*) &check_, NULL);
      uv_prepare_stop(&prepare_);
      uv_close((uv_handle_t*) &prepare_, NULL);
      uv_loop_close(loop_);
    }

    virtual State GetState() const = 0;

    void RunStage(const uv_run_mode mode = UV_RUN_NOWAIT);
  };
}

#endif //MCC_ENGINE_STAGE_H