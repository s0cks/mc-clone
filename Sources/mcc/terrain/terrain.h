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
    explicit TerrainVertexBuffer(const BufferObjectId id = kInvalidBufferObject):
      VertexBufferTemplate(id) {  
    }
    explicit TerrainVertexBuffer(const Vertex* vertices, const uint64_t num_vertices):
      VertexBufferTemplate(vertices, num_vertices) {
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
      glEnableVertexAttribArray(0);

      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, uv));
      glEnableVertexAttribArray(1);

      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, color));
      glEnableVertexAttribArray(2);
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

  class IndexBuffer : public IndexBufferTemplate<Index, mcc::kStaticUsage> {
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

#define FOR_EACH_TERRAIN_TEXTURE(V) \
  V(Concrete, "concrete.png")       \
  V(Wood, "wood.png")

  enum TerrainTexture {
#define DEFINE_TERRAIN_TEXTURE(Name, file) k##Name##Texture,
    FOR_EACH_TERRAIN_TEXTURE(DEFINE_TERRAIN_TEXTURE)
#undef DEFINE_TERRAIN_TEXTURE

    kNumberOfTerrainTextures,
    kDefaultTerrainTexture = kConcreteTexture,
  };

  static inline const char*
  GetTerrainTextureName(const TerrainTexture texture) {
    switch(texture) {
#define DEFINE_TOSTRING(Name, File) \
      case k##Name##Texture: return #Name;
      FOR_EACH_TERRAIN_TEXTURE(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      default:
        return "unknown";
    }
  }

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
    static IndexBuffer GetIbo();

    static TerrainTexture GetTexture(); //TODO: remove
    static void SetTexture(const TerrainTexture texture); //TODO: remove
  };
}

#endif //MCC_TERRAIN_H