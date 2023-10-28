#include "mcc/vao.h"

namespace mcc {
  Vao::Vao():
    id_() {
    glGenVertexArrays(1, &id_);
    CHECK_GL(FATAL);
  }

  void Vao::Link(Vbo& vbo, GLuint layout) {
    vbo.Bind();
    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(layout);
    CHECK_GL(FATAL);
    vbo.Unbind();
  }

  void Vao::Bind() const {
    glBindVertexArray(id_);
    CHECK_GL(FATAL);
  }

  void Vao::Unbind() const {
    glBindVertexArray(kInvalidVao);
    CHECK_GL(FATAL);
  }

  void Vao::Delete() {
    glDeleteVertexArrays(1, &id_);
    CHECK_GL(FATAL);
  }
}