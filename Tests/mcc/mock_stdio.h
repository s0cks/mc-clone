#ifndef MCC_MOCK_STDIO_H
#define MCC_MOCK_STDIO_H

#include <gmock/gmock.h>

namespace mcc {
  class MockStdio {
  public:
    MockStdio();
    ~MockStdio();
    MOCK_METHOD2(fopen, FILE*(const char* file, const char* mode));

#if defined(OS_IS_OSX) || defined(OS_IS_LINUX)
    MOCK_METHOD2(access, int(const char* file, const int mode));
    MOCK_METHOD2(stat, int(const char* file, struct stat* s));
#endif //OS_IS_OSX || OS_IS_LINUX

  };
}

#endif //MCC_MOCK_STDIO_H