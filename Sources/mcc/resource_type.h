#ifndef MCC_RESOURCE_TYPE_H
#define MCC_RESOURCE_TYPE_H

#include <iostream>
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

    static inline std::ostream&
    operator<<(std::ostream& stream, const Type& rhs) {
      switch(rhs) {
#define DEFINE_TOSTRING(Name) \
        case k##Name##Type: return stream << #Name;
        FOR_EACH_RESOURCE_TYPE(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
        case kUnknownType:
        default: return stream << "Unknown";
      }
    }
  }
}

#endif //MCC_RESOURCE_TYPE_H