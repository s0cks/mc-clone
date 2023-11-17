#ifndef MCC_VERTEX_H
#define MCC_VERTEX_H

#include <vector>
#include "mcc/gfx.h"

namespace mcc {
  namespace d2 {
    struct Vertex {
      glm::vec2 pos;
      glm::vec2 uv;
    public:
      friend std::ostream& operator<<(std::ostream& stream, const Vertex& rhs) {
        stream << "2d::Vertex(";
        stream << "pos=" << glm::to_string(rhs.pos) << ", ";
        stream << "uv=" << glm::to_string(rhs.uv);
        stream << ")";
        return stream;
      }
    };
    typedef std::vector<Vertex> VertexList;

    class VertexBuffer : public VertexBufferTemplate<Vertex, kStaticUsage> {
    public:
      enum Attributes {
        kPositionIndex = 0,
        kPositionOffset = offsetof(Vertex, pos),

        kUvIndex = 1,
        kUvOffset = offsetof(Vertex, uv),
      };

      DEFINE_VEC2F_VERTEX_BUFFER_ATTR(kPositionIndex, kPositionOffset, sizeof(Vertex), Position);
      DEFINE_VEC2F_VERTEX_BUFFER_ATTR(kUvIndex, kUvOffset, sizeof(Vertex), Uv);
    public:
      explicit VertexBuffer(const BufferObjectId id = kInvalidBufferObject):
        VertexBufferTemplate(id) {  
      }
      explicit VertexBuffer(const Vertex* vertices, const uint64_t num_vertices):
        VertexBufferTemplate(vertices, num_vertices) {
        PositionAttribute::Bind();
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
      static void OnPostInit();
    };

    Mesh* NewMesh(const Vertex* vertices, const uint64_t num_vertices);

    static inline Mesh*
    NewMesh(const VertexList& vertices) {
      return NewMesh(&vertices[0], vertices.size());
    }
  }

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

#endif //MCC_VERTEX_H