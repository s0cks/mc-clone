#include "mcc/skybox/skybox_builder.h"
#include <fmt/format.h>
#include "mcc/skybox/skybox.h"

namespace mcc::skybox {
  SkyboxBuilder::SkyboxBuilder():
    SkyboxBuilder(SkyboxMesh::New()) { //TODO: cache mesh
  }

  Skybox* SkyboxBuilder::Build() const {
    return Skybox::New(*this);
  }

  rx::observable<Skybox*> SkyboxBuilder::BuildAsync() const {
    return rx::observable<>::create<Skybox*>([this](rx::subscriber<Skybox*> s) {
      const auto skybox = Build();
      if(!skybox) {
        const auto err = fmt::format("failed to build new Skybox.");
        return s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
      }
      s.on_next(skybox);
      s.on_completed();
    });
  }
}