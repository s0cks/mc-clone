#ifndef MCC_VERTEX_H
#define MCC_VERTEX_H

#include <vector>
#include <cstdint>
#include <glog/logging.h>

#include "mcc/gfx.h"

namespace mcc {
  struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
  };

  static inline std::ostream& 
  operator<<(std::ostream& stream, const Vertex& rhs) {
    return stream << "Vertex()"; //TODO: implement
  }

  typedef std::vector<Vertex> VertexList;


  inline GLsizeiptr
  CalculateVertexBufferSize(const VertexList& list) {
    return list.size() * sizeof(Vertex);
  }

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