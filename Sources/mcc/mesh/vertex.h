#ifndef MCC_VERTEX_H
#define MCC_VERTEX_H

#include <vector>
#include <cstdint>
#include <glog/logging.h>

#include "mcc/gfx.h"

namespace mcc {
  typedef glm::vec3 Vertex;

  static constexpr const int8_t kVertexLength = 3;
  static constexpr const uint64_t kVertexSize = sizeof(float) * kVertexLength;

  typedef std::vector<Vertex> VertexList;

  static inline std::ostream&
  operator<<(std::ostream& stream, const VertexList& vertices) {
    stream << "[";
    auto iter = vertices.begin();
    do {
      Vertex vert = (*iter);
      stream << vert;
      stream << ", ";
    } while(++iter != vertices.end());
    stream << "]";
    return stream;
  }
}

#endif //MCC_VERTEX_H