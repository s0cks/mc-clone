#ifndef MCC_OS_THREAD_H
#define MCC_OS_THREAD_H

#include <string>
#include "mcc/rx.h"
#include "mcc/common.h"
#include "mcc/platform.h"

#ifdef OS_IS_LINUX
#include "mcc/os_thread_linux.h"
#elif OS_IS_OSX
#include "mcc/os_thread_osx.h"
#elif OS_IS_WINDOWS
#include "mcc/os_thread_windows.h"
#endif

namespace mcc {
  ThreadId GetCurrentThreadId();
  std::string GetThreadName(const ThreadId& thread);
  bool SetThreadName(const ThreadId& thread, const std::string& name);
  bool InitializeThreadLocal(ThreadLocalKey& key);
  bool SetCurrentThreadLocal(const ThreadLocalKey& key, const void* value);
  void* GetCurrentThreadLocal(const ThreadLocalKey& key);
  bool Start(ThreadId* thread, const std::string& name, const ThreadHandler& func, void* data);
  bool Join(const ThreadId& thread);
  bool Compare(const ThreadId& lhs, const ThreadId& rhs);
  int GetCurrentThreadCount();

  static inline std::string
  GetCurrentThreadName(){
    return GetThreadName(GetCurrentThreadId());
  }

  static inline bool
  SetCurrentThreadName(const std::string& name){
    return SetThreadName(GetCurrentThreadId(), name);
  }

  class MainThread {
    DEFINE_NON_INSTANTIABLE_TYPE(MainThread);
  private:
    static void SetThreadId(const ThreadId id);
  public:
    static void Init(const ThreadId id = GetCurrentThreadId());
    static ThreadId GetThreadId();

    static inline bool
    IsThread(const ThreadId id) {
      return Compare(GetThreadId(), id) == 0;
    }

    static inline bool
    IsCurrentThread() {
      return IsThread(GetCurrentThreadId());
    }
  };
}

#endif //MCC_OS_THREAD_H