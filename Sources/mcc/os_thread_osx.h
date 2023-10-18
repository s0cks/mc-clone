#ifndef MCC_OS_THREAD_OSX_H
#define MCC_OS_THREAD_OSX_H

#ifndef MCC_OS_THREAD_H
#error "Please #include <mcc/os_thread.h> instead of <mcc/os_thread_osx.h>"
#endif //MCC_OS_THREAD_H

#include <pthread.h>

namespace mcc {
  static const int kThreadNameMaxLength = 16;
  static const int kThreadMaxResultLength = 128;

  typedef pthread_key_t ThreadLocalKey;
  typedef pthread_t ThreadId;
  typedef void (*ThreadHandler)(void* data);
}

#endif //MCC_OS_THREAD_LINUX_H