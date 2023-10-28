#include "mcc/vao.h"

namespace mcc {
  Vao::Vao():
    id_(kInvalidVaoId) {
    glGenVertexArrays(1, &id_);
    CHECK_GL(ERROR);
  }

  void Vao::Link(Vbo& vbo, GLuint layout) {
    vbo.Bind();
    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    CHECK_GL(ERROR);
    glEnableVertexAttribArray(layout);
    CHECK_GL(ERROR);
    vbo.Unbind();
  }

  void Vao::Bind() {
    glBindVertexArray(id_);
    CHECK_GL(ERROR);
  }

  void Vao::Unbind() {
    glBindVertexArray(kInvalidVaoId);
    CHECK_GL(ERROR);
  }

  void Vao::Delete() {
    glDeleteVertexArrays(1, &id_);
    CHECK_GL(ERROR);
  }
}