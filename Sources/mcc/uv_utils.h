#ifndef MCC_UV_UTILS_H
#define MCC_UV_UTILS_H

#include <uv.h>
#include <functional>
#include <glog/logging.h>

#include "mcc/common.h"
#include "mcc/relaxed_atomic.h"

namespace mcc::uv {
#ifndef UV_OK
#define UV_OK 0
#endif //UV_OK

#define CHECK_UV_RESULT(Severity, Result, Message) \
  LOG_IF(Severity, (Result) != UV_OK) << (Message) << ": " << uv_strerror((Result));

  class Loop {
  public:
    enum RunMode {
      kRunDefault = UV_RUN_DEFAULT,
      kRunNoWait = UV_RUN_NOWAIT,
      kRunOnce = UV_RUN_ONCE,
    };
  protected:
    uv_loop_t* loop_;

    Loop(uv_loop_t* loop):
      loop_(loop) {
    }
  public:
    virtual ~Loop() = default;

    uv_loop_t* GetLoop() const {
      return loop_;
    }

    void Run(const RunMode mode = kRunDefault) {
      VLOG(3) << "running loop.";
      const auto result = uv_run(GetLoop(), static_cast<uv_run_mode>(mode));
      LOG_IF(ERROR, result == UV_OK) << "failed to run loop: " << uv_strerror(result);
      VLOG(3) << "done running loop.";
    }
  };

  class Handle {
  protected:
    Handle() = default;
  public:
    virtual ~Handle() = default;
  };

  template<typename H, typename C>
  class HandleTemplate : public Handle {
  protected:
    H handle_;
    C callback_;
  
    HandleTemplate(C callback):
      Handle(),
      handle_(),
      callback_(callback) {
    }

    inline uv_handle_t*
    handle() {
      return reinterpret_cast<uv_handle_t*>(&handle_);
    }

    template<typename T>
    static inline uv_handle_t*
    AsHandle(T* handle) {
      return reinterpret_cast<uv_handle_t*>(handle);
    }

    template<typename P, typename T>
    static inline T*
    GetHandleData(P* handle) {
      return reinterpret_cast<T*>(uv_handle_get_data(AsHandle<P>(handle)));
    }

    template<typename P>
    static inline void
    SetHandleData(P* handle, void* data) {
      uv_handle_set_data(AsHandle<P>(handle), data);
    }
  public:
    ~HandleTemplate() override = default;
  };
  
  typedef std::function<void()> PrepareCallback;

  class PrepareHandle : public HandleTemplate<uv_prepare_t, PrepareCallback> {
  private:
    static inline void
    OnPrepare(uv_prepare_t* h) {
      auto handle = reinterpret_cast<PrepareHandle*>(h->data);
      handle->callback_();
    }
  public:
    PrepareHandle(uv_loop_t* loop, PrepareCallback callback):
      HandleTemplate<uv_prepare_t, PrepareCallback>(callback) {
      CHECK_UV_RESULT(ERROR, uv_prepare_init(loop, &handle_), "failed to initialize uv_prepare_t");
      CHECK_UV_RESULT(ERROR, uv_prepare_start(&handle_, &OnPrepare), "failed to start uv_prepare_t");
      handle_.data = this;
    }
    ~PrepareHandle() override {
      CHECK_UV_RESULT(ERROR, uv_prepare_stop(&handle_), "failed to stop uv_prepare_t");
    }

    PrepareCallback callback() const {
      return callback_;
    }
  };

  typedef std::function<void()> IdleCallback;

  class IdleHandle : public HandleTemplate<uv_idle_t, IdleCallback> {
  protected:
    static inline void
    OnIdle(uv_idle_t* h) {
      auto handle = reinterpret_cast<IdleHandle*>(h->data);
      handle->callback_();
    }
  public:
    IdleHandle(uv_loop_t* loop, IdleCallback callback):
      HandleTemplate<uv_idle_t, IdleCallback>(callback) {
      CHECK_UV_RESULT(ERROR, uv_idle_init(loop, &handle_), "failed to initialize uv_idle_t");
      CHECK_UV_RESULT(ERROR, uv_idle_start(&handle_, &OnIdle), "failed to start uv_idle_t");
      handle_.data = this;
    }
    ~IdleHandle() override {
      CHECK_UV_RESULT(ERROR, uv_idle_stop(&handle_), "failed to stop uv_idle_t");
    }

    IdleCallback callback() const {
      return callback_;
    }
  };

  typedef std::function<void()> CheckCallback;

  class CheckHandle : public HandleTemplate<uv_check_t, CheckCallback> {
  protected:
    static inline void
    OnCheck(uv_check_t* h) {
      auto handle = reinterpret_cast<CheckHandle*>(h->data);
      handle->callback_();
    }
  public:
    CheckHandle(uv_loop_t* loop, CheckCallback callback):
      HandleTemplate<uv_check_t, CheckCallback>(callback) {
      CHECK_UV_RESULT(ERROR, uv_check_init(loop, &handle_), "failed to initialize uv_check_t");
      CHECK_UV_RESULT(ERROR, uv_check_start(&handle_, &OnCheck), "failed to start uv_check_t");
      handle_.data = this;
    }
    ~CheckHandle() override {
      CHECK_UV_RESULT(ERROR, uv_check_stop(&handle_), "fialed to stop uv_check_t");
    }

    CheckCallback callback() const {
      return callback_;
    }
  };

  static inline uint64_t
  Now() {
    return uv_hrtime();
  }

  template<typename H, typename D>
  static inline void
  SetHandleData(H* handle, const D* data) {
    return uv_handle_set_data((uv_handle_t*) handle, (void*) data);
  }

  template<typename H, typename D>
  static inline void
  SetHandleData(H& handle, const D* data) {
    return SetHandleData<H>(&handle, data);
  }

  template<typename D, typename H>
  static inline D*
  GetHandleData(const H* handle) {
    return (D*) uv_handle_get_data((uv_handle_t*) handle);
  }

  template<typename D>
  class AsyncHandle {
  public:
    typedef std::function<void(D*)> Callback;
  private:
    static inline void
    OnCall(uv_async_t* handle) {
      const auto async = GetHandleData<AsyncHandle>(handle);
      async->callback_(async->GetData());
    }
  protected:
    uv_async_t handle_;
    Callback callback_;
    uword data_;
  public:
    AsyncHandle(uv_loop_t* loop,
                const Callback& callback,
                const uword data = 0):
      handle_(),
      callback_(callback),
      data_((uword) data) {
      const auto err = uv_async_init(loop, &handle_, OnCall);
      LOG_IF(ERROR, err != UV_OK) << "uv_async_init failed: " << uv_strerror(err);
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

    inline void Call() {
      const auto err = uv_async_send(&handle_);
      LOG_IF(ERROR, err != UV_OK) << "uv_async_send failed: " << uv_strerror(err);
    }

    void operator() () {
      return Call();
    }
  };
}

#endif //MCC_UV_UTILS_H