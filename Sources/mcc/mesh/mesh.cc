#include "mcc/mesh/mesh.h"
#include "mcc/engine/engine.h"

namespace mcc::mesh {
  static VertexArrayObject vao_(kInvalidVertexArrayObject);

  static inline void
  InitializeVao() {
    vao_ = VertexArrayObject();
    VertexArrayObjectBindScope scope(vao_);
  }

  void Mesh::OnPostInit() {
    InitializeVao();
  }

  void Mesh::Init() {
    Engine::OnPostInit(&OnPostInit);
  }
  
  void Mesh::Render() {
    VertexArrayObjectBindScope vao(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vbo_.length());
    CHECK_GL(FATAL);
  }

  void IndexedMesh::Render() {
    VertexArrayObjectBindScope vao(vao_);
    glDrawElements(GL_TRIANGLES, ibo_.length(), ibo_.type(), 0);
    CHECK_GL(FATAL);
  }

  Mesh* NewMesh(const VertexList& vertices) {
    VertexArrayObjectBindScope vao(vao_);
    return new Mesh(vertices);
  }

  Mesh* NewMesh(const VertexList& vertices, const IndexList& indices) {
    VertexArrayObjectBindScope vao(vao_);
    return new IndexedMesh(vertices, indices);
  }
}