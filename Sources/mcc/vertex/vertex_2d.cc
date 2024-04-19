#include "mcc/vertex/vertex.h"
#include "mcc/engine/engine.h"

namespace mcc::d2 {
  static Vao kMeshVao;

  void Mesh::OnInit() {
    kMeshVao = VertexArrayObject::New();
  }

  void Mesh::Init() {
    const auto engine = engine::Engine::GetEngine();
    engine->OnPreInitEvent().subscribe([](engine::PreInitEvent* event) {
      Mesh::OnInit();
    });
  }

  Mesh::Mesh(const Vertex* vertices, const uint64_t num_vertices):
    vao(kMeshVao),
    vbo(vertices, num_vertices) {
  }

  void Mesh::Draw() {
    VertexArrayObjectScope vao_scope(vao);
    VertexBufferObjectDrawScope<VertexBuffer::PositionAttribute,
                                VertexBuffer::ColorAttribute,
                                VertexBuffer::UvAttribute> vbo_scope(vbo);
    vbo_scope.Draw(GL_TRIANGLES);
  }

  void IndexedMesh::Draw() {
    VertexArrayObjectScope vao_scope(vao);
    glDrawElements(GL_TRIANGLES, ibo_.length(), ibo_.type(), 0);
    CHECK_GL(FATAL);
  }

  Mesh* NewMesh(const Vertex* vertices, const uint64_t num_vertices) {
    VertexArrayObjectScope vao_scope(kMeshVao);
    return new Mesh(vertices, num_vertices);
  }

  Mesh* NewMesh(const Vertex* vertices, const uint64_t num_vertices,
                const u32::Index* indices, const uint64_t num_indices) {
    VertexArrayObjectScope vao_scope(kMeshVao);
    return new IndexedMesh(vertices, num_vertices, indices, num_indices);
  }
}