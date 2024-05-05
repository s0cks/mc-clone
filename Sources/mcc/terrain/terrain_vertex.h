#ifndef MCC_TERRAIN_VERTEX_H
#define MCC_TERRAIN_VERTEX_H

#include <vector>
#include "mcc/glm.h"
#include "mcc/color.h"
#include "mcc/vbo/vbo_attribute.h"

namespace mcc::terrain {
  struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
    Color color;

    Vertex() = default;
    Vertex(const Vertex& rhs) = default;
    ~Vertex() = default;

    Vertex& operator=(const Vertex& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const Vertex& rhs) {
      stream << "terrain::Vertex(";
      stream << "pos=" << glm::to_string(rhs.pos) << ", ";
      stream << "uv=" << glm::to_string(rhs.uv) << ", ";
      stream << "color=" << glm::to_string(rhs.color);
      stream << ")";
      return stream;
    }
  };

  class PosAttr : public vbo::Vec3fAttribute<0, sizeof(Vertex), offsetof(Vertex, pos)> {};
  class UvAttr : public vbo::Vec2fAttribute<1, sizeof(Vertex), offsetof(Vertex, uv)>{};
  class ColorAttr : public vbo::ColorAttribute<2, sizeof(Vertex), offsetof(Vertex, color)>{};

  typedef std::vector<Vertex> VertexList;

  static inline std::ostream&
  operator<<(std::ostream& stream, const VertexList& rhs) {
    stream << "[";
    for(auto idx = 0; idx < rhs.size(); idx++) {
      stream << rhs[idx];
      if((idx + 1) < rhs.size())
        stream << ", ";
    }
    stream << "]";
    return stream;
  }
}

#endif //MCC_TERRAIN_VERTEX_H