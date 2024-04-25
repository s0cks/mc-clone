#ifndef MCC_MESH_VERTEX_H
#define MCC_MESH_VERTEX_H

#include <vector>
#include <cstdint>
#include <iostream>
#include <glog/logging.h>

#include "mcc/gfx.h"

namespace mcc::mesh {
  struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 uv;
  };

  static inline std::ostream& 
  operator<<(std::ostream& stream, const Vertex& rhs) {
    stream << "mesh::Vertex(";
    stream << "pos=" << glm::to_string(rhs.pos) << ", ";
    stream << "uv=" << glm::to_string(rhs.uv) << ", ";
    stream << "normal=" << glm::to_string(rhs.normal) << ", ";
    stream << "color=" << glm::to_string(rhs.color) << ", ";
    stream << ")";
    return stream;
  }

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

#endif //MCC_MESH_VERTEX_H