#ifndef MCC_SKYBOX_BUILDER_H
#define MCC_SKYBOX_BUILDER_H

#include "mcc/rx.h"
#include "mcc/texture/texture.h"
#include "mcc/skybox/skybox_mesh.h"

namespace mcc {
  namespace skybox {
    class Skybox;
    class SkyboxBuilder {
      DEFINE_NON_COPYABLE_TYPE(SkyboxBuilder);
    protected:
      SkyboxMesh* mesh_;
      CubeMap* texture_;
    public:
      explicit SkyboxBuilder(SkyboxMesh* mesh):
        mesh_(mesh),
        texture_(nullptr) {
        MCC_ASSERT(mesh);
      }
      SkyboxBuilder();
      virtual ~SkyboxBuilder() = default;
      virtual Skybox* Build() const;
      virtual rx::observable<Skybox*> BuildAsync() const;

      CubeMap* GetTexture() const {
        return texture_;
      }

      void SetTexture(CubeMap* texture) {
        MCC_ASSERT(texture);
        texture_ = texture;
      }

      void SetTexture(const uri::Uri& uri) {
        const auto texture = CubeMap::New(uri);
        MCC_ASSERT(texture);
        return SetTexture(texture);
      }

      void SetTexture(const uri::basic_uri& uri) {
        return SetTexture(uri::Uri(uri));
      }

      SkyboxMesh* GetMesh() const {
        return mesh_;
      }

      void SetMesh(SkyboxMesh* mesh) {
        MCC_ASSERT(mesh);
        mesh_ = mesh;
      }
    };
  }
  using skybox::SkyboxBuilder;
}

#endif //MCC_SKYBOX_BUILDER_H