#ifndef MCC_COMMON_H
#define MCC_COMMON_H

#include <cstdio>
#include <cstdint>
#include <cassert>

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
}

#ifdef MCC_DEBUG

#define MCC_ASSERT(x) assert(x)

#else

#define MCC_ASSERT(x)

#endif //MCC_DEBUG

#endif //MCC_COMMON_H