#ifndef MCC_GUI_H
#define MCC_GUI_H

#include "mcc/gfx.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"

namespace mcc::gui {
  struct Vertex {
    glm::vec2 position;
    glm::vec2 uv;
    glm::vec4 color;

    friend std::ostream& operator<<(std::ostream& stream, const Vertex& rhs) {
      stream << "gui::Vertex(";
      stream << "pos=" << glm::to_string(rhs.position) << ", ";
      stream << "uv=" << glm::to_string(rhs.uv) << ", ";
      stream << "color=" << glm::to_string(rhs.color);
      stream << ")";
      return stream;
    }
  };

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

  // class VertexBuffer : public VertexBufferTemplate<Vertex, kDynamicUsage> {
  // public:
  //   explicit VertexBuffer(const BufferObjectId id = kInvalidBufferObject):
  //     VertexBufferObjectTemplate(id) {
  //   }
  //   explicit VertexBuffer(const VertexList& vertices):
  //     VertexBufferObject(kInvalidVertexBufferObject),
  //     num_vertices_(vertices.size()) {
  //     glGenBuffers(1, &id_);
  //     glBindBuffer(GL_ARRAY_BUFFER, id_);
  //     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
  //     glEnableVertexAttribArray(0);

  //     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, uv));
  //     glEnableVertexAttribArray(1);

  //     glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, color));
  //     glEnableVertexAttribArray(2);
  //   }
  //   VertexBuffer(const VertexBuffer& rhs):
  //     VertexBufferObject(rhs),
  //     num_vertices_(rhs.num_vertices_) {
  //   }
  //   ~VertexBuffer() override = default;

  //   void operator=(const VertexBuffer& rhs) {
  //     VertexBufferObject::operator=((const VertexBufferObject&) rhs);
  //     num_vertices_ = rhs.num_vertices_;
  //   }

  //   void operator=(const BufferObjectId& rhs) {
  //     VertexBufferObject::operator=(rhs);
  //   }
  // };

  typedef GLuint Index;
  typedef std::vector<Index> IndexList;

  static inline std::ostream&
  operator<<(std::ostream& stream, const IndexList& rhs) {
    stream << "(";
    for(auto idx = 0; idx < rhs.size(); idx++) {
      stream << rhs[idx];
      if((idx + 1) < rhs.size())
        stream << ", ";
    }
    stream << ")";
    return stream;
  }
}

#endif //MCC_GUI_H