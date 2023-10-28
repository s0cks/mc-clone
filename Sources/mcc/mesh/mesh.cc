#include "mcc/mesh/mesh.h"

namespace mcc::mesh {
  Mesh::Mesh(const std::vector<glm::vec3>& vertices):
    vao_(),
    vbo_(),
    num_vertices_(vertices.size()) {
    VaoScope vao(vao_);
    vbo_ = Vbo(vertices);
    vao_.Link(vbo_, 0);
    vbo_.Unbind();
  }

  void Mesh::Render() {
    VaoScope scope(vao_);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices_);
  }
}