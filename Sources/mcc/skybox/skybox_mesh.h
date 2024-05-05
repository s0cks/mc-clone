#ifndef MCC_SKYBOX_MESH_H
#define MCC_SKYBOX_MESH_H

#include "mcc/vao/vao.h"
#include "mcc/vbo/vbo.h"
#include "mcc/skybox/skybox_vertex.h"

namespace mcc::skybox {
  static constexpr const auto kSkyboxMeshVboBytes = 256;
  class SkyboxMesh {
  protected:
    Vao* vao_;
    Vbo* vbo_;

    SkyboxMesh(Vao* vao, Vbo* vbo):
      vao_(vao),
      vbo_(vbo) {
    }
  public:
    virtual ~SkyboxMesh() {
      delete vao_;
      delete vbo_;
    }
    virtual std::string ToString() const;

    Vao* GetVao() const {
      return vao_;
    }

    Vbo* GetVbo() const {
      return vbo_;
    }

    void Update(const VertexList& vertices);
  public:
    static SkyboxMesh* New();
  };
}

#endif //MCC_SKYBOX_MESH_H