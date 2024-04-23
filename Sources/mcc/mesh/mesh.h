#ifndef MCC_MESH_H
#define MCC_MESH_H

#include "mcc/gfx.h"
#include "mcc/common.h"
#include "mcc/vao/vao.h"
#include "mcc/ibo/ibo.h"
#include "mcc/mesh/vertex.h"

namespace mcc {
  class Renderer;
}

namespace mcc::mesh {
  class Mesh {
    friend class Renderer;
    friend class MeshTest;
  protected:
    Vao* vao_;
    VertexBuffer vbo_;

    static void InitializeBuiltinVaos();
  public:
    Mesh(Vao* vao, const VertexList& vertices):
      vao_(vao),
      vbo_(vertices) {
    }
    Mesh() = delete;
    virtual ~Mesh() = default;

    VertexBuffer vbo() const {
      return vbo_;
    }

    virtual void Render();
    virtual std::string ToString() const;

    friend std::ostream& operator<<(std::ostream& stream, const Mesh& rhs) {
      return stream << rhs.ToString();
    }
  private:
    static void OnPostInit();
  public:
    static void Init();
  };

  class IndexedMesh : public Mesh {
    friend class Renderer;
  protected:
    Ibo* ibo_;

    void Render() override;
  public:
    IndexedMesh() = delete;
    IndexedMesh(Vao* vao,
                const VertexList& vertices,
                const UIntIbo::IndexList& indices):
      Mesh(vao, vertices),
      ibo_(UIntIbo::New(indices, ibo::kDynamicDraw)) {
    }
    ~IndexedMesh() override = default;

    Ibo* ibo() const {
      return ibo_;
    }

    std::string ToString() const override;
  };

  Mesh* NewMesh(Vao* vao, const VertexList& vertices);
  Mesh* NewMesh(Vao* vao, const VertexList& vertices, const UIntIbo::IndexList& indices);
  
  Mesh* NewUVSphere(const uint64_t lat, const uint64_t lon);
  Mesh* NewIcosphere(const uint64_t resolution);
  Mesh* NewCube();
  Mesh* NewPlaneXY(const uint64_t xSize, const uint64_t ySize);
  Mesh* NewPlaneXZ(const uint64_t xSize, const uint64_t zSize);

  Mesh* LoadFrom(const std::string& filename);
}

#endif //MCC_MESH_H