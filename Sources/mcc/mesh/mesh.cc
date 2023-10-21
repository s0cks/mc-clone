#include "mcc/mesh/mesh.h"

namespace mcc {
  namespace mesh {
    void Mesh::Initialize(Mesh* mesh) {
      glGenVertexArrays(1, &mesh->vao_);
      glGenBuffers(1, &mesh->vbo_);
      glBindVertexArray(mesh->vao_);
      glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_);
      glBufferData(GL_ARRAY_BUFFER, CalculateVertexBufferSize(mesh->vertices_), &mesh->vertices_[0], GL_STATIC_DRAW);
      // position attribute
      glVertexAttribPointer(0, kVertexPosLength, GL_FLOAT, GL_FALSE, kVertexSize, (const GLvoid*) kVertexPosOffset);
      glEnableVertexAttribArray(0);
      // texture coord attribute
      glVertexAttribPointer(1, kVertexTexLength, GL_FLOAT, GL_FALSE, kVertexSize, (const GLvoid*) kVertexTexOffset);
      glEnableVertexAttribArray(1);
      glBindVertexArray(0);
    }

    void Mesh::Render() {
      glBindVertexArray(vao_);
      glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
      glBindVertexArray(0);
    }

    void IndexedMesh::Initialize(IndexedMesh* mesh) {
      glGenVertexArrays(1, &mesh->vao_);
      glGenBuffers(1, &mesh->vbo_);
      glGenBuffers(1, &mesh->ebo_);
      glBindVertexArray(mesh->vao_);
      glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_);
      glBufferData(GL_ARRAY_BUFFER, CalculateVertexBufferSize(mesh->vertices_), &mesh->vertices_[0], GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo_);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, CalculateIndexBufferSize(mesh->indices_), &mesh->indices_[0], GL_STATIC_DRAW);
      glVertexAttribPointer(0, kVertexLength, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glEnableVertexAttribArray(0);
      glBindVertexArray(0);
    }

    void IndexedMesh::Render()  {
      glBindVertexArray(vao_);
      glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices_.size()), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    }
  }
}