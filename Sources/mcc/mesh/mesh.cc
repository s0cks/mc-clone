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
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
      // tangent

      glBindVertexArray(0);

      const auto error = glGetError();
      if(error != GL_NO_ERROR)
        LOG(ERROR) << error;
    }

    void Mesh::Render() {
      glBindVertexArray(vao_);
      glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
      glBindVertexArray(0);
    }

#define CHECK_GL_ERROR(Error) ({ \
  const auto err = (Error); \
  LOG_IF(ERROR, err != GL_NO_ERROR) << "Gl Error: " << err; \
})

    void IndexedMesh::Initialize(IndexedMesh* mesh) {
      glGenVertexArrays(1, &mesh->vao_);
      CHECK_GL_ERROR(glGetError());
      glBindVertexArray(mesh->vao_);
      CHECK_GL_ERROR(glGetError());

      glGenBuffers(1, &mesh->vbo_); 
      CHECK_GL_ERROR(glGetError());
      glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_);
      CHECK_GL_ERROR(glGetError());
      glBufferData(GL_ARRAY_BUFFER, CalculateVertexBufferSize(mesh->vertices_), &mesh->vertices_[0], GL_STATIC_DRAW);
      CHECK_GL_ERROR(glGetError());

      // position
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
      glEnableVertexAttribArray(0);
      CHECK_GL_ERROR(glGetError());
      // normal
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
      glEnableVertexAttribArray(1);
      CHECK_GL_ERROR(glGetError());
      // tex
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
      glEnableVertexAttribArray(2);
      CHECK_GL_ERROR(glGetError());

      glGenBuffers(1, &mesh->ebo_);
      CHECK_GL_ERROR(glGetError());
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo_);
      CHECK_GL_ERROR(glGetError());
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, CalculateIndexBufferSize(mesh->indices_), &mesh->indices_[0], GL_STATIC_DRAW);
      CHECK_GL_ERROR(glGetError());

      glBindVertexArray(0);
      CHECK_GL_ERROR(glGetError());
    }

    void IndexedMesh::Render()  {
      glBindVertexArray(vao_);
      CHECK_GL_ERROR(glGetError());
      glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices_.size()), GL_UNSIGNED_INT, 0);
      CHECK_GL_ERROR(glGetError());
      glBindVertexArray(0);
      CHECK_GL_ERROR(glGetError());
    }
  }
}