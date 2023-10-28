#include "mcc/vbo.h"

namespace mcc {
  Vbo::Vbo(const GLfloat* vertices, GLsizeiptr size):
    id_(kInvalidVboId) {
    glGenBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  }

  void Vbo::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    CHECK_GL(ERROR);
  }

  void Vbo::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL(ERROR);
  }

  void Vbo::Delete() {
    glDeleteBuffers(1, &id_);
    CHECK_GL(ERROR);
  }
}