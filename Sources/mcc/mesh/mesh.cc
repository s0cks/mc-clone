#include "mcc/mesh/mesh.h"

namespace mcc {
  namespace mesh {
    void Mesh::Initialize(Mesh* mesh) {
      glGenVertexArrays(1, &mesh->vao_);
      glGenBuffers(1, &mesh->vbo_);
      glBindVertexArray(mesh->vao_);
      glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_);
      glBufferData(GL_ARRAY_BUFFER, CalculateVertexBufferSize(mesh->vertices_), &mesh->vertices_[0], GL_STATIC_DRAW);
      // position
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
      // normal
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
      // tex
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
      // tangent
      glEnableVertexAttribArray(3);
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
      // bitangent
      glEnableVertexAttribArray(4);
      glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

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

      // position
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
      // normal
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
      // tex
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
      // tangent
      glEnableVertexAttribArray(3);
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
      // bitangent
      glEnableVertexAttribArray(4);
      glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

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