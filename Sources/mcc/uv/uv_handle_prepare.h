#ifndef MCC_UV_HANDLE_H
#error "Please #include <mcc/uv/handle.h> instead."
#endif //MCC_UV_HANDLE_H

#ifndef MCC_UV_HANDLE_PREPARE_H
#define MCC_UV_HANDLE_PREPARE_H

#include "mcc/uv/uv_handle.h"
#include "mcc/uv/uv_status.h"

namespace mcc::uv {
  class PrepareListener {
    friend class PrepareHandle;
  protected:
    PrepareListener() = default;
    virtual void OnPrepare() = 0;
  public:
    virtual ~PrepareListener() = default;
  };

  class PrepareHandleBase : public HandleBaseTemplate<uv_prepare_t> {
  protected:
    PrepareHandleBase() = default;

    static inline Status
    InitPrepareHandle(uv_loop_t* loop, uv_prepare_t* handle) {
      return Status(uv_prepare_init(loop, handle));
    }

    static inline Status
    StartPrepareHandle(uv_prepare_t* handle, uv_prepare_cb cb) {
      return Status(uv_prepare_start(handle, cb));
    }

    static inline Status
    StopPrepareHandle(uv_prepare_t* handle) {
      return Status(uv_prepare_stop(handle));
    }
  public:
    ~PrepareHandleBase() override {
      StopPrepareHandle(&handle_);
    }

    virtual void Start() = 0;

    virtual void Stop() {
      StopPrepareHandle(&handle_);
    }
  };

  class PrepareHandle : public PrepareHandleBase {
  private:
    static inline void
    OnPrepare(uv_prepare_t* handle) {
      const auto listener = GetHandleData<PrepareListener>(handle);
      MCC_ASSERT(listener);
      return listener->OnPrepare();
    }
  public:
    explicit PrepareHandle(uv_loop_t* loop,
                           const PrepareListener* listener,
                           const bool start = true):
      PrepareHandleBase() {
      SetHandleData(handle_, listener);
      InitPrepareHandle(loop, &handle_);
      Start();
    }
    ~PrepareHandle() override = default;

    void Start() override {
      StartPrepareHandle(&handle_, &OnPrepare);
    }
  };
}

#endif //MCC_UV_HANDLE_PREPARE_H