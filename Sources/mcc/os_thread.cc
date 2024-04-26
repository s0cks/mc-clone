#include "mcc/os_thread.h"

#include <glog/logging.h>
#include "mcc/relaxed_atomic.h"

namespace mcc {
  static RelaxedAtomic<ThreadId> kMainThreadId;

  void MainThread::SetThreadId(const ThreadId id) {
    kMainThreadId = id;
  }

  ThreadId MainThread::GetThreadId() {
    return (ThreadId) kMainThreadId;
  }

  void MainThread::Init(const ThreadId id) {
    DLOG(INFO) << "initializing main thread on thread: " << id;
    SetThreadId(id);
  }
}