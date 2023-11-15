#ifndef MCC_RESOURCE_TYPE_H
#define MCC_RESOURCE_TYPE_H

#include "mcc/platform.h"

namespace mcc {
#define FOR_EACH_RESOURCE_TYPE(V) \
  V(Shader)                       \
  V(Texture)                      \
  V(Material)

  namespace resource {
    enum Type : uint8_t {
      kUnknownType = 0,
#define DEFINE_RESOURCE_TYPE(Name) k##Name##Type,
      FOR_EACH_RESOURCE_TYPE(DEFINE_RESOURCE_TYPE)
#undef DEFINE_RESOURCE_TYPE
    };
  }
}

#endif //MCC_RESOURCE_TYPE_H