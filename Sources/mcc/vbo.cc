#include "mcc/vbo.h"

namespace mcc {
  Vbo::Vbo(const GLfloat* vertices, GLsizeiptr size):
    id_(kInvalidVboId) {
    glGenBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const GLvoid*) 0);
    glEnableVertexAttribArray(0);
  }

  void Vbo::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    CHECK_GL(FATAL);
  }

  void Vbo::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL(FATAL);
  }

  void Vbo::Delete() {
    glDeleteBuffers(1, &id_);
    CHECK_GL(FATAL);
  }
}