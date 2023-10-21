#ifndef MCC_MESH_H
#define MCC_MESH_H

#include <vector>
#include "mcc/gfx.h"
#include "mcc/common.h"
#include "mcc/mesh/index.h"
#include "mcc/mesh/vertex.h"

namespace mcc {
  namespace mesh {
    class Mesh {
      DEFINE_NON_COPYABLE_TYPE(Mesh);
    protected:
      GLuint vao_;
      GLuint vbo_;
      VertexList vertices_;

      Mesh(const VertexList& vertices):
        vao_(0),
        vbo_(0),
        vertices_(vertices) {
      }

      static void Initialize(Mesh* mesh);
    public:
      virtual ~Mesh() = default;

      GLuint vao() const {
        return vao_;
      }

      GLuint vbo() const {
        return vbo_;
      }

      VertexList vertices() const {
        return vertices_;
      }

      virtual void Render();
    public:
      static Mesh* New(const VertexList& vertices) {
        const auto mesh = new Mesh(vertices);
        Mesh::Initialize(mesh);
        return mesh;
      }
    };

    class IndexedMesh : public Mesh {
      DEFINE_NON_COPYABLE_TYPE(IndexedMesh);
    protected:
      GLuint ebo_;
      IndexList indices_;

      IndexedMesh(const VertexList& vertices, IndexList indices):
        Mesh(vertices),
        indices_(indices) {      
      }

      static void Initialize(IndexedMesh* mesh);
    public:
      ~IndexedMesh() override = default;

      IndexList indices() const {
        return indices_;
      }

      virtual void Render() override;
    public:
      static Mesh* New(const VertexList& vertices, IndexList indices) {
        const auto mesh = new IndexedMesh(vertices, indices);
        IndexedMesh::Initialize(mesh);
        return mesh;
      }
    };
  }
}

#endif //MCC_MESH_H