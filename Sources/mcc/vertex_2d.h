#ifndef MCC_VERTEX_H
#error "Please #include <mcc/vertex.h> instead."
#endif //MCC_VERTEX_H

#ifndef MCC_VERTEX_2D_H
#define MCC_VERTEX_2D_H

#include <vector>
#include "mcc/gfx.h"
#include "mcc/vao.h"

namespace mcc::d2 {
  struct Vertex {
    glm::vec2 pos;
    glm::vec2 uv;
    glm::u8vec4 color;
  public:
    friend std::ostream& operator<<(std::ostream& stream, const Vertex& rhs) {
      stream << "d2::Vertex(";
      stream << "pos=" << glm::to_string(rhs.pos) << ", ";
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

  class VertexBuffer : public VertexBufferTemplate<Vertex, kStreamUsage> {
  public:
    enum Attributes {
      kPositionIndex = 0,
      kPositionOffset = offsetof(Vertex, pos),

      kUvIndex = 1,
      kUvOffset = offsetof(Vertex, uv),

      kColorIndex = 2,
      kColorOffset = offsetof(Vertex, color),
    };

    DEFINE_VEC2F_VERTEX_BUFFER_ATTR(kPositionIndex, kPositionOffset, sizeof(Vertex), Position);
    DEFINE_VEC2F_VERTEX_BUFFER_ATTR(kUvIndex, kUvOffset, sizeof(Vertex), Uv);
    class ColorAttribute : public VertexBufferAttribute<kColorIndex, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), kColorOffset>{};
  public:
    explicit VertexBuffer(const BufferObjectId id):
      VertexBufferTemplate(id) {  
    }
    VertexBuffer():
      VertexBufferTemplate() {
      PositionAttribute::Bind();
      UvAttribute::Bind();
      ColorAttribute::Bind();
    }
    explicit VertexBuffer(const Vertex* vertices, const uint64_t num_vertices):
      VertexBufferTemplate(vertices, num_vertices) {
      PositionAttribute::Bind();
      UvAttribute::Bind();
      ColorAttribute::Bind();
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
  };
  DEFINE_RESOURCE_SCOPE(VertexBuffer);

  struct Mesh {
    VertexArrayObject vao;
    VertexBuffer vbo;

    Mesh(const Vertex* vertices, const uint64_t num_vertices);
    explicit Mesh(const VertexList& vertices):
      Mesh(&vertices[0], vertices.size()) {
    }
    ~Mesh() = default;
    void Draw();

    friend std::ostream& operator<<(std::ostream& stream, const Mesh& rhs) {
      stream << "d2::Mesh(";
      stream << "vao=" << rhs.vao << ", ";
      stream << "vbo=" << rhs.vbo;
      stream << ")";
      return stream;
    }

    static void Init();
  private:
    static void OnInit();
  };

  Mesh* NewMesh(const Vertex* vertices, const uint64_t num_vertices);

  static inline Mesh*
  NewMesh(const VertexList& vertices) {
    return NewMesh(&vertices[0], vertices.size());
  }
}

#endif //MCC_VERTEX_2D_H