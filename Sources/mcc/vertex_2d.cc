#include "mcc/vertex.h"
#include "mcc/engine/engine.h"

namespace mcc::d2 {
  static Vao kMeshVao;

  void Mesh::OnInit() {
    kMeshVao = VertexArrayObject::New();
  }

  void Mesh::Init() {
    Engine::OnInit(&OnInit);
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

  Mesh* NewMesh(const Vertex* vertices, const uint64_t num_vertices) {
    VertexArrayObjectScope vao_scope(kMeshVao);
    return new Mesh(vertices, num_vertices);
  }
}