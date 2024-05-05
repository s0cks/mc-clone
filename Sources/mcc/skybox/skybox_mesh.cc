#include "mcc/skybox/skybox_mesh.h"

#include "mcc/thread_local.h"
#include "mcc/vbo/vbo_builder.h"
#include "mcc/skybox/skybox_vertex.h"

namespace mcc::skybox {
  static ThreadLocal<Vao> skybox_vao_;

  static inline Vao*
  GetSkyboxVao() {
    if(skybox_vao_)
      return skybox_vao_.Get();
    const auto vao = Vao::New();
    skybox_vao_.Set(vao);
    return vao;
  }

  static inline Vbo*
  CreateSkyboxVbo() {
    vbo::VboBuilder<skybox::Vertex,
                    skybox::PosAttr> builder(64, vbo::kDynamicDraw);
    return builder.Build()
      .as_blocking()
      .first();
  }

  std::string SkyboxMesh::ToString() const {
    std::stringstream ss;
    ss << "SkyboxMesh(";
    ss << "vao=" << GetVao()->ToString() << ", ";
    ss << "vbo=" << GetVbo()->ToString();
    ss << ")";
    return ss.str();
  }

  SkyboxMesh* SkyboxMesh::New() {
    return new SkyboxMesh(GetSkyboxVao(), CreateSkyboxVbo());
  }
}