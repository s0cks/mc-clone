#include "mcc/ibo.h"

namespace mcc {
  Ibo::Ibo(const GLubyte* indices, GLsizeiptr size):
    id_(kInvalidIbo) {
    glGenBuffers(1, &id_);
    CHECK_GL(FATAL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    CHECK_GL(FATAL);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
  }

  void Ibo::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    CHECK_GL(FATAL);
  }

  void Ibo::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    CHECK_GL(FATAL);
  }

  void Ibo::Delete() {
    glDeleteBuffers(1, &id_);
    CHECK_GL(FATAL);
  }
}