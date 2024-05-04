#include "mcc/material/material_loader.h"
#include "mcc/material/material.h"

namespace mcc::material {
  Material* MaterialDirectoryLoader::LoadMaterial() const {
    DLOG(INFO) << "resolving material components in " << GetRoot() << "....";

    DLOG(INFO) << "resolved " << components_.size() << " material components in " << GetRoot() << ".";
    return Material::New(name_, components_);
  }
}