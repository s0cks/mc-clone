#include "mcc/material.h"

namespace mcc {
  namespace material {
    MaterialPtr JsonMaterialLoader::LoadMaterial() {
      auto material = std::make_shared<Material>();
      if(!doc_.HasMember("name")) {
        DLOG(ERROR) << "no 'name' field found in material document.";
        return nullptr;
      }
      const auto name = std::string(doc_["name"].GetString());
      return std::make_shared<Material>(Material {
        .name = name,
        .location = root_,
        .albedo = ParseMaterialComponent("albedo"),
        .ao = ParseMaterialComponent("ao"),
        .height = ParseMaterialComponent("height"),
        .metallic = ParseMaterialComponent("metallic"),
        .normal = ParseMaterialComponent("normal"),
        .roughness = ParseMaterialComponent("roughness"),
      });
    }

    MaterialPtr Material::LoadFrom(const std::string& filename) {
      return JsonMaterialLoader::Load(filename);
    }
  }
}