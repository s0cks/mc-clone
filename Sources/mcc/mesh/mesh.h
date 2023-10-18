#ifndef MCC_MESH_H
#define MCC_MESH_H

#include <vector>
#include "mcc/gfx.h"
#include "mcc/common.h"
#include "mcc/mesh/index.h"
#include "mcc/mesh/vertex.h"

namespace mcc {
  class Mesh {
  protected:
    VertexList vertices_;
    IndexList indices_;
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;

    void InitializeMesh();

    inline GLsizeiptr
    GetVertexBufferSize() {
      return vertices_.size() * sizeof(Vertex);
    }

    inline void
    BindVertexBuffer() {
      glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    }

    inline void
    LoadVertexBufferData() {
      BindVertexBuffer();
      glBufferData(GL_ARRAY_BUFFER, GetVertexBufferSize(), vertices_.data(), GL_STATIC_DRAW);
    }

    inline void
    GenerateVertexBufferData() {
      glGenBuffers(1, &vbo_);
      LoadVertexBufferData();
    }

    inline GLsizeiptr
    GetIndexBufferSize() {
      return indices_.size() * sizeof(Index);
    }

    inline void
    BindIndexBuffer() {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    }

    inline void
    LoadIndexBufferData() {
      BindIndexBuffer();
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndexBufferSize(), indices_.data(), GL_STATIC_DRAW);
    }

    inline void
    GenerateIndexBufferData() {
      glGenBuffers(1, &ebo_);
      LoadIndexBufferData();
    }
  public:
    Mesh(VertexList vertices, IndexList indices);
    virtual ~Mesh() = default;
    DEFINE_NON_COPYABLE_TYPE(Mesh);

    VertexList vertices() const {
      return vertices_;
    }

    IndexList indices() const {
      return indices_;
    }
  public:
    void Render();
  };
}

#endif //MCC_MESH_H