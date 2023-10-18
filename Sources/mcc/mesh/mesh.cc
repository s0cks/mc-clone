#include "mcc/mesh/mesh.h"

namespace mcc {
  Mesh::Mesh(VertexList vertices, IndexList indices):
    vertices_(vertices),
    indices_(indices) {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, GetVertexBufferSize(), &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndexBufferSize(), &indices_[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, kVertexLength, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
  }

  void Mesh::Render()  {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices_.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}