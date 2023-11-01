#ifndef MCC_VERTEX_H
#define MCC_VERTEX_H

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

  class VertexBuffer : public VertexBufferTemplate<Vertex> {
  public:
    explicit VertexBuffer(const BufferObjectId id = kInvalidBufferObject):
      VertexBufferTemplate(id) {
    }
    explicit VertexBuffer(const Vertex* vertices, const uint64_t num_vertices):
      VertexBufferTemplate(vertices, num_vertices) {
      // pos
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
      CHECK_GL(FATAL);
      glEnableVertexAttribArray(0);
      CHECK_GL(FATAL);

      // normal
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, normal));
      CHECK_GL(FATAL);
      glEnableVertexAttribArray(1);
      CHECK_GL(FATAL);

      // color
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, color)); 
      CHECK_GL(FATAL);
      glEnableVertexAttribArray(2);
      CHECK_GL(FATAL);

      // uv
      glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, uv));
      CHECK_GL(FATAL);
      glEnableVertexAttribArray(3);
      CHECK_GL(FATAL);
      Unbind();
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

#endif //MCC_VERTEX_H