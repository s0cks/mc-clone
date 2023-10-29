#ifndef MCC_MESH_H
#define MCC_MESH_H

#include "mcc/gfx.h"
#include "mcc/common.h"
#include "mcc/mesh/vertex.h"
#include "mcc/mesh/index.h"

namespace mcc {
  class Renderer;
}

namespace mcc::mesh {
  class Mesh {
    friend class Renderer;
  protected:
    VertexBuffer vbo_;
  public:
    Mesh() = delete;
    Mesh(const VertexList& vertices):
      vbo_(vertices) {
    }
    virtual ~Mesh() = default;

    VertexBuffer vbo() const {
      return vbo_;
    }

    virtual void Render();
  private:
    static void OnPostInit();
  public:
    static void Init();
  };

  class IndexedMesh : public Mesh {
    friend class Renderer;
  protected:
    IndexBuffer ibo_;

    void Render() override;
  public:
    IndexedMesh() = delete;
    IndexedMesh(const VertexList& vertices,
                const IndexList& indices):
      Mesh(vertices),
      ibo_(indices) {
    }
    ~IndexedMesh() override = default;

    IndexBuffer ibo() const {
      return ibo_;
    }
  };

  Mesh* NewMesh(const VertexList& vertices);
  Mesh* NewMesh(const VertexList& vertices, const IndexList& indices);
}

#endif //MCC_MESH_H