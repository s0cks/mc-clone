#ifndef MCC_UV_HANDLE_H
#error "Please #include <mcc/uv/uv_handle.h> instead."
#endif //MCC_UV_HANDLE_H

#ifndef MCC_UV_HANDLE_IDLE_H
#define MCC_UV_HANDLE_IDLE_H

#include "mcc/uv/uv_handle.h"
#include "mcc/uv/uv_status.h"

namespace mcc::uv {
  class IdleListener {
    friend class IdleHandle;
  protected:
    IdleListener() = default;
    virtual void OnIdle() = 0;
  public:
    virtual ~IdleListener() = default;
  };

  class IdleHandleBase : public HandleBaseTemplate<uv_idle_t> {
  protected:
    IdleHandleBase() = default;

    static inline Status
    InitIdleHandle(uv_loop_t* loop, uv_idle_t* handle) {
      return Status(uv_idle_init(loop, handle));
    }

    static inline Status
    StartIdleHandle(uv_idle_t* handle, uv_idle_cb cb) {
      return Status(uv_idle_start(handle, cb));
    }

    static inline Status
    StopIdleHandle(uv_idle_t* handle) {
      return Status(uv_idle_stop(handle));
    }
  public:
    ~IdleHandleBase() override {
      StopIdleHandle(&handle_);
    }

    virtual void Start() = 0;

    virtual void Stop() {
      StopIdleHandle(&handle_);
    }
  };

  class IdleHandle : public IdleHandleBase {
  private:
    static inline void
    OnIdle(uv_idle_t* handle) {
      const auto listener = GetHandleData<IdleListener>(handle);
      listener->OnIdle();
    }
  public:
    IdleHandle(uv_loop_t* loop,
               const IdleListener* listener,
               const bool start = true):
      IdleHandleBase() {
      SetHandleData(handle_, listener);
      InitIdleHandle(loop, &handle_);
      if(start)
        Start();
    }
    ~IdleHandle() override = default;

    void Start() override {
      StartIdleHandle(&handle_, &OnIdle);
    }
  };
}

#endif //MCC_UV_HANDLE_IDLE_H