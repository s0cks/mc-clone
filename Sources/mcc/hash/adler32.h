#ifndef MCC_ADLER32_H
#define MCC_ADLER32_H

#include <string>
#include <cstring>
#include "mcc/platform.h"

namespace mcc {
  uint32_t adler32(const uint8_t* bytes, const uword num_bytes);

  static inline uint32_t
  adler32(const char* value, const uword length) {
    return adler32((const uint8_t*) value, length);
  }

  static inline uint32_t
  adler32(const char* value) {
    return adler32(value, strlen(value));
  }

  static inline uint32_t
  adler32(const std::string& value) {
    return adler32(value.data(), value.length());
  }
}

#endif //MCC_ADLER32_H