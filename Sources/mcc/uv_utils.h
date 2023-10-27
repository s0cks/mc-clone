#ifndef MCC_UV_UTILS_H
#define MCC_UV_UTILS_H

#include <uv.h>
#include <functional>
#include <glog/logging.h>

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
}

#endif //MCC_UV_UTILS_H