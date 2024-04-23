#ifndef MCC_IBO_INDEX_H
#define MCC_IBO_INDEX_H

#include "mcc/gfx.h"

namespace mcc::ibo {
  class UnsignedByteIndex {
  public:
    typedef GLubyte Type;
    typedef std::vector<Type> List;
  };

  class UnsignedShortIndex {
  public:
    typedef GLushort Type;
    typedef std::vector<Type> List;
  };

  class UnsignedIntIndex {
  public:
    typedef GLuint Type;
    typedef std::vector<Type> List;
  };
}

#endif //MCC_IBO_INDEX_H