#ifndef MCC_INDEX_H
#define MCC_INDEX_H

#include <vector>
#include "mcc/gfx.h"

namespace mcc {
  typedef GLuint Index;

  typedef std::vector<Index> IndexList;

  static inline std::ostream&
  operator<<(std::ostream& stream, const IndexList& rhs) {
    stream << "[";
    IndexList::const_iterator iter = rhs.begin();
    do {
      stream << (*iter) << ", ";
    } while(++iter != rhs.end());
    stream << "]";
    return stream;
  }
}

#endif //MCC_INDEX_H