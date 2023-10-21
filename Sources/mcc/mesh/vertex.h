#ifndef MCC_VERTEX_H
#define MCC_VERTEX_H

#include <vector>
#include <cstdint>
#include <glog/logging.h>

#include "mcc/gfx.h"

namespace mcc {
  struct Vertex {
    glm::vec3 pos;
    glm::vec2 tex;
  };

  static inline std::ostream& 
  operator<<(std::ostream& stream, const Vertex& rhs) {
    return stream << "Vertex()"; //TODO: implement
  }

  static constexpr const GLint kVertexPosLength = 3;
  static constexpr const GLint kVertexPosSize = kVertexPosLength * sizeof(float);
  static constexpr const GLint kVertexPosOffset = 0;

  static constexpr const GLint kVertexTexLength = 2;
  static constexpr const GLint kVertexTexSize = kVertexTexLength * sizeof(float);
  static constexpr const GLint kVertexTexOffset = kVertexPosOffset + kVertexPosSize;

  static constexpr const GLint kVertexLength = kVertexPosLength + kVertexTexLength;
  static constexpr const GLint kVertexSize = kVertexPosSize + kVertexTexSize;

  typedef std::vector<Vertex> VertexList;


  inline GLsizeiptr
  CalculateVertexBufferSize(VertexList& list) {
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