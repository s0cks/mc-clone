#ifndef MCC_TEXTURE_CACHE_FLAGS_H
#define MCC_TEXTURE_CACHE_FLAGS_H

#include <gflags/gflags.h>

namespace mcc::texture {
  static constexpr const char* kDefaultTextureCacheDir = "";
  DECLARE_string(texture_cache_dir);

  static constexpr const uint32_t kDefaultTextureCacheSize = 32;
  DECLARE_uint32(texture_cache_size);
}

#endif //MCC_TEXTURE_CACHE_FLAGS_H