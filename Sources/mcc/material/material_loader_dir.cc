#include "mcc/material/material_loader_dir.h"
#include "mcc/material/material.h"

#include "mcc/file_resolver.h"

namespace mcc::material {
  rx::observable<MaterialComponent> MaterialDirectoryLoader::GetMaterialComponents() const {
    return rx::observable<>::create<MaterialComponent>([this](rx::subscriber<MaterialComponent> s) {
      DLOG(INFO) << "resolving material components from " << GetRoot() << "....";
    });
  }
}