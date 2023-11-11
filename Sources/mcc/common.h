#ifndef MCC_COMMON_H
#define MCC_COMMON_H

#include <cstdio>
#include <cstdint>
#include <cassert>
#include <string>

#include "mcc/platform.h"

#if defined(OS_IS_LINUX) || defined(OS_IS_OSX)
#include <unistd.h>
#else
#error "unsupported operating system"
#endif

#define DEFINE_NON_COPYABLE_TYPE(Name) \
  public:                              \
    Name(const Name& rhs) = delete;    \
    Name& operator=(const Name& rhs) = delete;

#define DEFINE_NON_INSTANTIABLE_TYPE(Name) \
  public:                                  \
    Name() = delete;                       \
    ~Name() = delete;                      \
    DEFINE_NON_COPYABLE_TYPE(Name);

namespace mcc {
  static inline uint64_t
  GetFilesize(FILE* file) {
    const auto pos = ftell(file);
    fseek(file, 0, SEEK_END);
    const auto sz = ftell(file);
    fseek(file, pos, SEEK_SET);
    return sz;
  }

  static inline uword
  RoundUpPow2(uword x){
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
#if defined(ARCHITECTURE_IS_X64) || defined(ARCHITECTURE_IS_ARM64)
    x = x | (x >> 32);
#endif
    return x + 1;
  }

  static inline bool
  FileExists(const std::string& name){
#if defined(OS_IS_OSX) || defined(OS_IS_LINUX)
    return access(name.data(), F_OK) == 0;
#else
#error "unsupported operating system"
#endif
  }

  static inline bool
  StartsWith(const std::string& str, const std::string& prefix) {
     return str.size() >= prefix.size() 
         && str.compare(0, prefix.size(), prefix) == 0;
  }

  static bool
  EndsWith(const std::string& str, const std::string& suffix){
    return str.size() >= suffix.size() 
        && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
  }
}

#ifdef MCC_DEBUG

#define MCC_ASSERT(x) assert(x)

#else

#define MCC_ASSERT(x)

#endif //MCC_DEBUG

#endif //MCC_COMMON_H