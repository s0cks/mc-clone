#include "mcc/vertex.h"
#include "mcc/engine/engine.h"

namespace mcc::d2 {
  static VertexArrayObject k2dMeshVao(kInvalidVertexArrayObject);

  void Mesh::OnInit() {
    k2dMeshVao = VertexArrayObject();
  }

  void Mesh::Init() {
    Engine::OnInit(&OnInit);
  }

  Mesh::Mesh(const Vertex* vertices, const uint64_t num_vertices):
    vao(k2dMeshVao),
    vbo(vertices, num_vertices) {
  }

  void Mesh::Draw() {
    VertexArrayObjectScope vao_scope(vao);
    VertexBuffer::PositionAttribute::Enable();
    VertexBuffer::UvAttribute::Enable();
    vbo.Draw(GL_TRIANGLES);
  }

  Mesh* NewMesh(const Vertex* vertices, const uint64_t num_vertices) {
    VertexArrayObjectScope vao_scope(k2dMeshVao);
    return new Mesh(vertices, num_vertices);
  }
}