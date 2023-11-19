#ifndef MCC_VERTEX_H
#define MCC_VERTEX_H

#include <vector>
#include "mcc/gfx.h"

namespace mcc {
  namespace d3 {
    struct Vertex {
      glm::vec3 pos;
      glm::vec2 uv;
      glm::vec3 normal;

      friend std::ostream& operator<<(std::ostream& stream, const Vertex& rhs) {
        stream << "3d::Vertex(";
        stream << "pos=" << glm::to_string(rhs.pos) << ", ";
        stream << "uv=" << glm::to_string(rhs.uv) << ", ";
        stream << "normal=" << glm::to_string(rhs.normal);
        stream << ")";
        return stream;
      }
    };

    typedef std::vector<Vertex> VertexList;
  }
}

#include "mcc/vertex_2d.h"

#endif //MCC_VERTEX_H