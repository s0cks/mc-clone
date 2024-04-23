#ifndef MCC_IBO_INDEX_H
#define MCC_IBO_INDEX_H

#include "mcc/gfx.h"

namespace mcc::ibo {
  class UnsignedByteIndex {
  public:
    typedef GLubyte Type;
    typedef std::vector<Type> List;

    static inline constexpr const char*
    GetName() {
      return "UnsignedByte";
    }
  };

  class UnsignedShortIndex {
  public:
    typedef GLushort Type;
    typedef std::vector<Type> List;

    static inline constexpr const char*
    GetName() {
      return "UnsignedShort";
    }
  };

  class UnsignedIntIndex {
  public:
    typedef GLuint Type;
    typedef std::vector<Type> List;

    static inline constexpr const char*
    GetName() {
      return "UnsignedInt";
    }
  };
}

#endif //MCC_IBO_INDEX_H