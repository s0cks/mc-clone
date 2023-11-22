#ifndef MCC_MESH_VERTEX_H
#define MCC_MESH_VERTEX_H

#include <vector>
#include <cstdint>
#include <iostream>
#include <glog/logging.h>
#include "mcc/gfx.h"
#include "mcc/vertex_buffer.h"

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

  class VertexBuffer : public VertexBufferTemplate<Vertex> {
  public:
    enum VertexLayout {
      kPosIndex = 0,
      kPosOffset = offsetof(Vertex, pos),

      kNormalIndex = 1,
      kNormalOffset = offsetof(Vertex, normal),

      kColorIndex = 2,
      kColorOffset = offsetof(Vertex, color),

      kUvIndex = 3,
      kUvOffset = offsetof(Vertex, uv),
    };

    DEFINE_VEC3F_VERTEX_BUFFER_ATTR(kPosIndex, kPosOffset, sizeof(Vertex), Position);
    DEFINE_VEC3F_VERTEX_BUFFER_ATTR(kNormalIndex, kNormalOffset, sizeof(Vertex), Normal);
    DEFINE_VEC3F_VERTEX_BUFFER_ATTR(kColorIndex, kColorOffset, sizeof(Vertex), Color);
    DEFINE_VEC3F_VERTEX_BUFFER_ATTR(kUvIndex, kUvOffset, sizeof(Vertex), Uv);
  public:
    explicit VertexBuffer(const BufferObjectId id = kInvalidBufferObject):
      VertexBufferTemplate(id) {
    }
    explicit VertexBuffer(const Vertex* vertices, const uint64_t num_vertices):
      VertexBufferTemplate(vertices, num_vertices) {
      PositionAttribute::Bind();
      NormalAttribute::Bind();
      ColorAttribute::Bind();
      UvAttribute::Bind();
    }
    explicit VertexBuffer(const VertexList& vertices):
      VertexBuffer(&vertices[0], vertices.size()) {
    }
    VertexBuffer(const VertexBuffer& rhs):
      VertexBufferTemplate(rhs) {
    }
    ~VertexBuffer() override = default;
    
    void operator=(const VertexBuffer& rhs) {
      VertexBufferTemplate::operator=(rhs);
    }

    void operator=(const BufferObjectId& rhs) {
      BufferObject::operator=(rhs);
    }

    friend std::ostream& operator<<(std::ostream& stream, const VertexBuffer& rhs) {
      stream << "mesh::VertexBuffer(";
      stream << "id=" << rhs.id() << ", ";
      stream << "vertex_size=" << rhs.vertex_size() << ", ";
      stream << "length=" << rhs.length() << ", ";
      stream << "size=" << rhs.size();
      stream << "usage=" << rhs.usage() << ", ";
      stream << "target=" << rhs.target();
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_MESH_VERTEX_H