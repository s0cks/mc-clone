#ifndef MCC_UV_HANDLE_CHECK_H
#define MCC_UV_HANDLE_CHECK_H

namespace mcc::uv {
  class CheckListener {
    friend class CheckHandle;
  protected:
    CheckListener() = default;
    virtual void OnCheck() = 0;
  public:
    virtual ~CheckListener() = default;
  };

  class CheckHandleBase : public HandleBaseTemplate<uv_check_t> {
  protected:
    CheckHandleBase() = default;

    static inline Status
    InitCheckHandle(uv_loop_t* loop, uv_check_t* handle) {
      return Status(uv_check_init(loop, handle));
    }

    static inline Status
    StartCheckHandle(uv_check_t* handle, uv_check_cb cb) {
      return Status(uv_check_start(handle, cb));
    }

    static inline Status
    StopCheckHandle(uv_check_t* handle) {
      return Status(uv_check_stop(handle));
    }
  public:
    ~CheckHandleBase() override {
      StopCheckHandle(&handle_);
    }

    virtual void Start() = 0;

    virtual void Stop() {
      StopCheckHandle(&handle_);
    }
  };

  class CheckHandle : public CheckHandleBase {
  private:
    static inline void
    OnCheck(uv_check_t* handle) {
      const auto listener = GetHandleData<CheckListener>(handle);
      listener->OnCheck();
    }
  public:
    explicit CheckHandle(uv_loop_t* loop,
                        const CheckListener* listener,
                        const bool start = true):
      CheckHandleBase() {
      SetHandleData(handle_, listener);
      InitCheckHandle(loop, &handle_);
      if(start)
        Start();
    }
    ~CheckHandle() override = default;

    void Start() override {
      StartCheckHandle(&handle_, &OnCheck);
    }
  };
}

#endif //MCC_UV_HANDLE_CHECK_H