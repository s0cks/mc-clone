#ifndef MCC_FLAGS_H
#define MCC_FLAGS_H

#include <gflags/gflags.h>

namespace mcc {
  DECLARE_string(resources);

  DECLARE_bool(fullscreen);
  DECLARE_uint32(width);
  DECLARE_uint32(height);
}

#endif //MCC_FLAGS_H