#ifndef MCC_TERRAIN_H
#define MCC_TERRAIN_H

#include <vector>
#include <iostream>

#include "mcc/gfx.h"
#include "mcc/common.h"

namespace mcc {
  class Renderer;
}

namespace mcc::terrain {
  struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 color;

    friend std::ostream& operator<<(std::ostream& stream, const Vertex& rhs) {
      stream << "Vertex(";
      stream << "pos=" << rhs.pos << ", ";
      stream << "uv=" << rhs.uv << ", ";
      stream << "color=" << rhs.color;
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

  class TerrainVertexBuffer : public VertexBufferObject {
  private:
    uint32_t num_vertices_;
  public:
    explicit TerrainVertexBuffer(const VertexBufferObjectId id = kInvalidVertexBufferObject):
      VertexBufferObject(id),
      num_vertices_(0) {
    }
    explicit TerrainVertexBuffer(const VertexList& vertices):
      VertexBufferObject(kInvalidVertexBufferObject),
      num_vertices_(vertices.size()) {
      glGenBuffers(1, &id_);
      glBindBuffer(GL_ARRAY_BUFFER, id_);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
      glEnableVertexAttribArray(0);

      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, uv));
      glEnableVertexAttribArray(1);

      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, color));
      glEnableVertexAttribArray(2);
    }
    TerrainVertexBuffer(const TerrainVertexBuffer& rhs):
      VertexBufferObject(rhs.id_),
      num_vertices_(rhs.num_vertices_) {
    }
    ~TerrainVertexBuffer() override = default;

    uint32_t length() const {
      return num_vertices_;
    }
    
    void operator=(const TerrainVertexBuffer& rhs) {
      VertexBufferObject::operator=((const VertexBufferObject&) rhs);
      num_vertices_ = rhs.num_vertices_;
    }

    void operator=(const VertexBufferObjectId& rhs) {
      VertexBufferObject::operator=(rhs);
    }
  };

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

  class TerrainIndexBuffer : public IndexBufferObject {
  private:
    uint32_t num_indices_;
  public:
    explicit TerrainIndexBuffer(const IndexBufferObjectId id = kInvalidIndexBufferObject):
      IndexBufferObject(id),
      num_indices_(0) {
    }
    explicit TerrainIndexBuffer(const IndexList& indices):
      IndexBufferObject(kInvalidIndexBufferObject),
      num_indices_(indices.size()) {
      glGenBuffers(1, &id_);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Index), &indices[0], GL_STATIC_DRAW);
    }
    TerrainIndexBuffer(const TerrainIndexBuffer& rhs):
      IndexBufferObject(rhs.id_),
      num_indices_(rhs.num_indices_) {
    }
    ~TerrainIndexBuffer() override = default;

    uint32_t length() const {
      return num_indices_;
    }
    
    void operator=(const TerrainIndexBuffer& rhs) {
      IndexBufferObject::operator=((const IndexBufferObject&) rhs);
      num_indices_ = rhs.num_indices_;
    }

    void operator=(const IndexBufferObjectId& rhs) {
      IndexBufferObject::operator=(rhs);
    }
  };

  class Terrain {
    friend class mcc::Renderer;
    DEFINE_NON_COPYABLE_TYPE(Terrain);
  private:

  public:
    Terrain() = delete;
    virtual ~Terrain() = default;

  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();

    static void Render(const glm::mat4 projection, const glm::mat4 view);
  public:
    static void Init();
    static VertexArrayObject GetVao();
    static TerrainVertexBuffer GetVbo();
    static TerrainIndexBuffer GetIbo();
  };
}

#endif //MCC_TERRAIN_H