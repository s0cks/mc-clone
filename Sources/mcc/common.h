#ifndef MCC_COMMON_H
#define MCC_COMMON_H

#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <glog/logging.h>
#include <algorithm>

#include "platform.h"

#if defined(OS_IS_LINUX) || defined(OS_IS_OSX)
#include <unistd.h>
#include <sys/stat.h>
#else
#error "unsupported operating system"
#endif

#if defined(_M_X64) || defined(__x86_64__)

#define ARCH_IS_X64 1

#elif defined(_M_IX86) || defined(__i386__)

#define ARCH_IS_X32 1

#elif defined(__ARMEL__)

#define ARCH_IS_ARM 1

#elif defined(__aarch64__)

#define ARCH_IS_ARM64 1

#endif


#ifdef MCC_DEBUG

#include <cassert>
#define MCC_ASSERT(x) assert((x));

#else

#define MCC_ASSERT(x) \

#endif //MCC_DEBUG

#ifndef UNALLOCATED
#define UNALLOCATED 0
#endif //UNALLOCATED

#define DEFINE_NON_COPYABLE_TYPE(Name) \
  public:                              \
    Name(const Name& rhs) = delete;    \
    Name& operator=(const Name& rhs) = delete;

#define DEFINE_NON_INSTANTIABLE_TYPE(Name) \
  public:                                  \
    Name() = delete;                       \
    ~Name() = delete;                      \
    DEFINE_NON_COPYABLE_TYPE(Name);

#define DEFINE_DEFAULT_COPYABLE_TYPE(Name)            \
  public:                                             \
    Name(const Name& rhs) = default;                  \
    Name& operator=(const Name& rhs) = default;

#define NOT_IMPLEMENTED(Level) \
  LOG(Level) << __FUNCTION__ << " is not implemented!";

namespace mcc {
  template<typename A, typename B>
  std::optional<B> map(std::optional<A> a, std::function<std::optional<B>(const A&)> f) {
    if(a.has_value())
      return f(a.value());
    return std::optional<B>{};
  }

  static inline uint64_t
  GetFilesize(FILE* file) {
    const auto pos = ftell(file);
    fseek(file, 0, SEEK_END);
    const auto sz = ftell(file);
    fseek(file, pos, SEEK_SET);
    return sz;
  }

  static inline uword
  RoundUpPow2(uword x) {
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
  IsDirectory(const std::string& filename) {
#if defined(OS_IS_OSX) || defined(OS_IS_LINUX) 
    struct stat s;
    if(stat(filename.c_str(), &s) != 0)
      return false;
    return S_ISDIR(s.st_mode);
#else
#error "unsupported operating system"
#endif

  }

  static inline bool
  DeleteDirectory(const std::string& name){
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

  static inline bool
  CharEqualsIgnoreCase(const char& lhs, const char& rhs) {
    return std::tolower(static_cast<unsigned char>(lhs)) == std::tolower(static_cast<unsigned char>(rhs));
  }

  static inline bool
  EqualsIgnoreCase(const std::string& lhs, const std::string& rhs) {
    return lhs.size() == rhs.size()
        && std::equal(rhs.begin(), rhs.end(), lhs.begin(), lhs.end(), CharEqualsIgnoreCase);
  }

  static inline void
  ToLowercase(std::string& value) {
    std::transform(std::begin(value), std::end(value), std::begin(value), [](const unsigned char c) {
      return std::tolower(c);
    });
  }
}

#endif //MCC_COMMON_H