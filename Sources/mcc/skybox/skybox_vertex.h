#ifndef MCC_SKYBOX_VERTEX_H
#define MCC_SKYBOX_VERTEX_H

#include "mcc/glm.h"
#include "mcc/vbo/vbo_attribute.h"

namespace mcc::skybox {
  struct Vertex {
    glm::vec3 pos;

    Vertex() = default;
    Vertex(const Vertex& rhs) = default;
    ~Vertex() = default;

    Vertex& operator=(const Vertex& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const Vertex& rhs) {
      stream << "skybox::Vertex(";
      stream << "pos=" << glm::to_string(rhs.pos);
      stream << ")";
      return stream;
    }
  };
  class PosAttr : public vbo::Vec3fAttribute<0, sizeof(Vertex), offsetof(Vertex, pos)> {};

  typedef std::vector<Vertex> VertexList;

  static inline std::ostream&
  operator<<(std::ostream& stream, const VertexList& rhs) {
    stream << "[";
    for(auto idx = 0; idx < rhs.size(); idx++) {
      const auto& v = rhs[idx];
      stream << v;
      if(idx < (rhs.size() - 1))
        stream << ", ";
    }
    stream << "]";
    return stream;
  }

  void GenerateSkyboxVertices(VertexList& vertices);
}

#endif //MCC_SKYBOX_VERTEX_H