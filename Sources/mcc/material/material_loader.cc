#include "mcc/material/material_loader.h"
#include "mcc/material/material.h"

namespace mcc::material {
  Material* MaterialLoader::LoadMaterial() const {
    const auto name = GetMaterialName();
    MaterialComponentSet components;
    GetMaterialComponents()
      .as_blocking()
      .subscribe([&components](const MaterialComponent& component) {
        const auto result = components.insert(component);
        LOG_IF(WARNING, !result.second) << "failed to insert " << component << " into material components.";
      });
    return Material::New(name, components);
  }
}