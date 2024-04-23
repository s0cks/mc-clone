#ifndef MCC_IBO_TYPE_H
#define MCC_IBO_TYPE_H

#include <vector>
#include "mcc/gfx.h"
#include "mcc/common.h"

namespace mcc::ibo {
#define FOR_EACH_IBO_TYPE(V)                              \
  V(UnsignedByte,   uint8_t,    GL_UNSIGNED_BYTE)         \
  V(UnsignedShort,  uint16_t,   GL_UNSIGNED_SHORT)        \
  V(UnsignedInt,    uint32_t,   GL_UNSIGNED_INT)

#define DEFINE_IBO_TYPE(Name, RawType, GlType)                  \
  class Name {                                                  \
  public:                                                       \
    typedef RawType Type;                                       \
    static inline constexpr const char*                         \
    GetName() {                                                 \
      return #Name;                                             \
    }                                                           \
    static inline constexpr const GLenum                        \
    GetGlType() {                                               \
      return (GlType);                                          \
    }                                                           \
    static inline const uint64_t                                \
    GetSize() {                                                 \
      return sizeof(uint64_t);                                  \
    }                                                           \
    static inline const uint64_t                                \
    CalculateBufferSize(const uint64_t num_indices) {           \
      return GetSize() * num_indices;                           \
    }                                                           \
    static inline const uint64_t                                \
    CalculateBufferSize(const std::vector<Type>& indices) {     \
      return CalculateBufferSize(indices.size());               \
    }                                                           \
  public:                                                       \
    DEFINE_NON_INSTANTIABLE_TYPE(Name);                         \
  };
  FOR_EACH_IBO_TYPE(DEFINE_IBO_TYPE)
#undef DEFINE_IBO_TYPE
}

#endif //MCC_IBO_TYPE_H