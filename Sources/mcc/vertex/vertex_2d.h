#ifndef MCC_VERTEX_H
#error "Please #include <mcc/vertex.h> instead."
#endif //MCC_VERTEX_H

#ifndef MCC_VERTEX_2D_H
#define MCC_VERTEX_2D_H

#include <vector>
#include "mcc/gfx.h"
#include "mcc/vao.h"
#include "mcc/uri.h"
#include "mcc/pipeline.h"
#include "mcc/index_buffer.h"
#include "mcc/vertex/vertex_buffer.h"
#include "mcc/program/program_pipeline.h"

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

  class Mesh {
  protected:
    Vao vao;
    VertexBuffer vbo;
  public:
    Mesh(const Vertex* vertices, const uint64_t num_vertices);
    explicit Mesh(const VertexList& vertices):
      Mesh(&vertices[0], vertices.size()) {
    }
    virtual ~Mesh() = default;
    virtual void Draw();

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

  class IndexedMesh : public Mesh {
    friend class Mesh;
  protected:
    u32::IndexBuffer ibo_;
  public:
    IndexedMesh(const Vertex* vertices, const uint64_t num_vertices,
                const uint32_t* indices, const uint64_t num_indices):
      Mesh(vertices, num_vertices),
      ibo_(indices, num_indices) {
    }
    explicit IndexedMesh(const VertexList& vertices,
                         const u32::IndexList& indices):
      Mesh(&vertices[0], vertices.size()),
      ibo_(&indices[0], indices.size()) {
    }
    virtual ~IndexedMesh() = default;

    void Draw() override;
  };

  class RenderMeshPipeline : public Pipeline {
  protected:
    Mesh* mesh_;
  public:
    explicit RenderMeshPipeline(Mesh* mesh, program::ApplyProgramPipeline* shader = nullptr):
      Pipeline(),
      mesh_(mesh) {
      if(!shader)
        return;
      AddChild(shader);
    }
    RenderMeshPipeline(Mesh* mesh,
                       const uri::Uri& uri,
                       program::ApplyProgramPipeline::SetUniformsCallback apply = program::ApplyProgramPipeline::kDoNothing):
      RenderMeshPipeline(mesh, program::ApplyProgramPipeline::New(uri, apply)) {
    }
    RenderMeshPipeline(Mesh* mesh,
                       const uri::basic_uri& uri,
                       program::ApplyProgramPipeline::SetUniformsCallback apply = program::ApplyProgramPipeline::kDoNothing):
      RenderMeshPipeline(mesh, program::ApplyProgramPipeline::New(uri, apply)) {
    }
    ~RenderMeshPipeline() override = default;

    Mesh* mesh() const {
      return mesh_;
    }

    void Render() override {
      DLOG(INFO) << "rendering mesh....";
      InvertedCullFaceScope cull_face;
      RenderChildren();
      mesh()->Draw();
    }
  };

  Mesh* NewMesh(const Vertex* vertices, const uint64_t num_vertices);
  Mesh* NewMesh(const Vertex* vertices, const uint64_t num_vertices, const u32::Index* indices, const uint64_t num_indices);

  static inline Mesh*
  NewMesh(const VertexList& vertices) {
    return NewMesh(&vertices[0], vertices.size());
  }

  static inline Mesh*
  NewMesh(const VertexList& vertices, const u32::IndexList& indices) {
    return NewMesh(&vertices[0], vertices.size(), &indices[0], indices.size());
  }
}

#endif //MCC_VERTEX_2D_H