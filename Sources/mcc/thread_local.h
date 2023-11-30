#ifndef MCC_THREAD_LOCAL_H
#define MCC_THREAD_LOCAL_H

#include <glog/logging.h>
#include "mcc/os_thread.h"

namespace mcc {
  template<typename T>
  class ThreadLocal {
  protected:
    ThreadLocalKey key_;
  public:
    ThreadLocal():
      key_() {
        LOG_IF(FATAL, !InitializeThreadLocal(key_));  
    }
    virtual ~ThreadLocal() = default;

    bool Set(T* value) {
      return SetCurrentThreadLocal(key_, reinterpret_cast<void*>(value));
    }

    T* Get() {
      return reinterpret_cast<T*>(GetCurrentThreadLocal(key_));
    }

    operator bool () const {
      return Get() != nullptr;
    }
  };
}

#endif //MCC_THREAD_LOCAL_H