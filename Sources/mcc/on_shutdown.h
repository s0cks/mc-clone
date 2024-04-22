#ifndef MCC_ON_SHUTDOWN_H
#define MCC_ON_SHUTDOWN_H

#include "mcc/uv/utils.h"

namespace mcc {
  class ShutdownListenerHandle;
  class ShutdownListener {
    friend class ShutdownListenerHandle;
  protected:
    ShutdownListener() = default;
    virtual void OnShutdown() = 0;
  public:
    virtual ~ShutdownListener() = default;
  };

  class ShutdownListenerHandle : public uv::AsyncHandleBase {
  private:
    static inline void
    OnCall(uv_async_t* handle) {
      const auto listener = GetHandleData<ShutdownListener>(handle);
      listener->OnShutdown();
    }
  public:
    ShutdownListenerHandle(uv_loop_t* loop, const ShutdownListener* listener):
      uv::AsyncHandleBase(loop, &OnCall) {
      SetHandleData(&handle_, listener);
    }
    ~ShutdownListenerHandle() override = default;
  };
}

#endif //MCC_ON_SHUTDOWN_H