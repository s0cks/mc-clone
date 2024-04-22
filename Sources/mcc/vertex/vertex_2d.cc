#include "mcc/vertex/vertex.h"
#include "mcc/engine/engine.h"

#include "mcc/vao/vao_scope.h"

namespace mcc::d2 {
  static Vao* kMeshVao;

  Mesh::Mesh(const Vertex* vertices, const uint64_t num_vertices):
    vao(kMeshVao),
    vbo(vertices, num_vertices) {
  }

  void Mesh::Draw() {
    VaoBindScope vao_scope(vao);
    VertexBufferObjectDrawScope<VertexBuffer::PositionAttribute,
                                VertexBuffer::ColorAttribute,
                                VertexBuffer::UvAttribute> vbo_scope(vbo);
    vbo_scope.Draw(GL_TRIANGLES);
  }

  class IndexBufferDrawScope {
  protected:
    const IndexBufferObject& ibo_;
  public:
    IndexBufferDrawScope(const IndexBufferObject& ibo):
      ibo_(ibo) {
      ibo_.Bind();
    }
    ~IndexBufferDrawScope() {
      ibo_.Unbind();
    }

    void Draw(const GLenum mode) {
      glDrawElements(GL_TRIANGLES, ibo_.length(), ibo_.type(), 0);
      CHECK_GL(FATAL);
    }
  };

  void IndexedMesh::Draw() {
    VaoBindScope vao_scope(vao);
    IndexBufferDrawScope ibo(ibo_);
    ibo.Draw(GL_TRIANGLES);
  }

  Mesh* NewMesh(const Vertex* vertices, const uint64_t num_vertices) {
    if(!kMeshVao || !kMeshVao->IsValid()) {
      kMeshVao = Vao::New();
      LOG_IF(FATAL, !kMeshVao->IsValid()) << "failed to initialize mesh vao.";
    }

    VaoBindScope vao_scope(kMeshVao);
    return new Mesh(vertices, num_vertices);
  }

  Mesh* NewMesh(const Vertex* vertices, const uint64_t num_vertices,
                const u32::Index* indices, const uint64_t num_indices) {
    if(!kMeshVao || !kMeshVao->IsValid()) {
      kMeshVao = Vao::New();
      LOG_IF(FATAL, !kMeshVao->IsValid()) << "failed to initialize mesh vao.";
      DLOG(INFO) << "created mesh vao: " << (*kMeshVao);
    }

    VaoBindScope vao_scope(kMeshVao);
    return new IndexedMesh(vertices, num_vertices, indices, num_indices);
  }
}