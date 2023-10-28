#include "mcc/mesh/mesh.h"

namespace mcc::mesh {
  Mesh::Mesh(const std::vector<glm::vec3>& vertices):
    vao_(),
    ibo_(0),
    vbo_(0),
    num_vertices_(vertices.size()),
    num_indices_(0) {
    DLOG(INFO) << "mesh vao: " << vao_;
    vao_.Bind();
    vbo_ = Vbo(vertices);
    DLOG(INFO) << "mesh vbo: " << vbo_;
    vao_.Link(vbo_, 0);
    vao_.Unbind();
    vbo_.Unbind();
  }

  Mesh::Mesh(const std::vector<glm::vec3>& vertices, const std::vector<GLubyte>& indices):
    vao_(),
    ibo_(0),
    vbo_(0),
    num_vertices_(vertices.size()),
    num_indices_(indices.size()) {
    DLOG(INFO) << "mesh vao: " << vao_;
    vao_.Bind();
    vbo_ = Vbo(vertices);
    DLOG(INFO) << "mesh vbo: " << vbo_;
    ibo_ = Ibo(indices);
    DLOG(INFO) << "mesh ibo: " << ibo_;
    vao_.Link(vbo_, 0);
    vao_.Unbind();
    vbo_.Unbind();
    ibo_.Unbind();
  }

  void Mesh::Render() {
    VaoScope scope(vao_);
    if(num_indices_ > 0) {
      glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_BYTE, 0);
    } else {
      glDrawArrays(GL_TRIANGLES, 0, num_vertices_);
    }
    CHECK_GL(FATAL);
  }
}