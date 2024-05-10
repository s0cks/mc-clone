#include "mcc/skybox/skybox_mesh.h"

#include "mcc/thread_local.h"
#include "mcc/vao/vao_scope.h"
#include "mcc/vbo/vbo_scope.h"
#include "mcc/vbo/vbo_builder.h"

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
    VertexList vertices;
    GenerateSkyboxVertices(vertices);
    vbo::VboBuilder<skybox::Vertex,
                    skybox::PosAttr> builder(vertices, gfx::kDynamicDrawUsage);
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

  void SkyboxMesh::Update(const VertexList& vertices) {
    vbo::VboUpdateScope update(vbo_);
    update.Update(vertices);
  }

  SkyboxMesh* SkyboxMesh::New() {
    const auto vao = GetSkyboxVao();
    vao::VaoBindScope scope(vao);
    return new SkyboxMesh(vao, CreateSkyboxVbo());
  }
}