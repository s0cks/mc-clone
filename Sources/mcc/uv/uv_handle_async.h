#ifndef MCC_UV_HANDLE_H
#error "Please #include <mcc/uv/handle.h> instead."
#endif //MCC_UV_HANDLE_H

#ifndef MCC_UV_HANDLE_ASYNC_H
#define MCC_UV_HANDLE_ASYNC_H

#include "mcc/common.h"
#include "mcc/uv/uv_handle.h"
#include "mcc/uv/uv_status.h"

namespace mcc::uv {
  class AsyncHandleBase : public HandleBase {
  protected:
    uv_async_t handle_;

    AsyncHandleBase(uv_loop_t* loop, uv_async_cb callback):
      handle_() {
      const auto err = uv_async_init(loop, &handle_, callback);
      LOG_IF(ERROR, err != UV_OK) << "uv_async_init failed: " << uv_strerror(err);
    }
  public:
    virtual ~AsyncHandleBase() = default;//TODO: remove async handle from loop?

    virtual void Call() {
      const auto err = uv_async_send(&handle_);
      LOG_IF(ERROR, err != UV_OK) << "uv_async_send failed: " << uv_strerror(err);
    }
  };

  template<typename D>
  class AsyncHandle : public AsyncHandleBase {
  public:
    typedef std::function<void(D*)> Callback;
  private:
    static inline void
    OnCall(uv_async_t* handle) {
      const auto async = GetHandleData<AsyncHandle>(handle);
      async->callback_(async->GetData());
    }
  protected:
    Callback callback_;
    uword data_;
  public:
    AsyncHandle(uv_loop_t* loop,
                const Callback& callback,
                const uword data = 0):
      AsyncHandleBase(loop, &OnCall),
      callback_(callback),
      data_((uword) data) {
    }
    AsyncHandle(uv_loop_t* loop,
                const Callback& callback,
                const D* data = 0):
      AsyncHandle(loop, callback, (uword) data) {
    }
    ~AsyncHandle() = default; //TODO: remove async handle from loop?

    D* GetData() const {
      return (D*) data_;
    }

    void operator() () {
      return Call();
    }
  };
}

#endif //MCC_UV_HANDLE_ASYNC_H