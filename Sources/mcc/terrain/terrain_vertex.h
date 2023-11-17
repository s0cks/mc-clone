#ifndef MCC_TERRAIN_VERTEX_H
#define MCC_TERRAIN_VERTEX_H

#include "mcc/gfx.h"

namespace mcc::terrain {
  struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 color;

    friend std::ostream& operator<<(std::ostream& stream, const Vertex& rhs) {
      stream << "Vertex(";
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

  class TerrainVertexBuffer : public VertexBufferTemplate<Vertex, kStaticUsage> {
  public:
    DEFINE_VEC3F_VERTEX_BUFFER_ATTR(0, sizeof(Vertex), Position);
    DEFINE_VEC2F_VERTEX_BUFFER_ATTR(1, sizeof(Vertex), Uv);
    DEFINE_VEC3F_VERTEX_BUFFER_ATTR(2, sizeof(Vertex), Color); //TODO: remove
  public:
    explicit TerrainVertexBuffer(const BufferObjectId id = kInvalidBufferObject):
      VertexBufferTemplate(id) {  
    }
    explicit TerrainVertexBuffer(const Vertex* vertices, const uint64_t num_vertices):
      VertexBufferTemplate(vertices, num_vertices) {
      PositionAttribute::Bind((const GLvoid*) 0);
      UvAttribute::Bind((const GLvoid*) offsetof(Vertex, uv));
      ColorAttribute::Bind((const GLvoid*) offsetof(Vertex, color));
    }
    explicit TerrainVertexBuffer(const VertexList& vertices):
      TerrainVertexBuffer(&vertices[0], vertices.size()) {  
    }
    TerrainVertexBuffer(const TerrainVertexBuffer& rhs):
      VertexBufferTemplate(rhs) {
    }
    ~TerrainVertexBuffer() override = default;
    
    void operator=(const TerrainVertexBuffer& rhs) {
      VertexBufferTemplate::operator=(rhs);
    }

    void operator=(const BufferObjectId& rhs) {
      BufferObject::operator=(rhs);
    }
  };
  DEFINE_RESOURCE_SCOPE(TerrainVertexBuffer);

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

  class IndexBuffer : public IndexBufferTemplate<Index, GL_UNSIGNED_INT, mcc::kStaticUsage> {
  public:
    explicit IndexBuffer(const BufferObjectId id = kInvalidBufferObject):
      IndexBufferTemplate(id) {
    }
    explicit IndexBuffer(const Index* indices, const uint64_t num_indices):
      IndexBufferTemplate(indices, num_indices) {
    }
    explicit IndexBuffer(const IndexList& indices):
      IndexBuffer(&indices[0], indices.size()) {
    }
    IndexBuffer(const IndexBuffer& rhs) = default;
    ~IndexBuffer() override = default;
    
    void operator=(const IndexBuffer& rhs) {
      IndexBufferTemplate::operator=(rhs);
    }

    void operator=(const BufferObjectId& rhs) {
      BufferObject::operator=(rhs);
    }
  };
  DEFINE_RESOURCE_SCOPE(IndexBuffer);
}

#endif //MCC_TERRAIN_VERTEX_H