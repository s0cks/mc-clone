#include "mcc/material.h"

namespace mcc {
  namespace material {
    Material* JsonMaterialLoader::LoadMaterial() {
      if(!doc_.HasMember("name")) {
        DLOG(ERROR) << "no 'name' field found in material document.";
        return nullptr;
      }
      const auto name = std::string(doc_["name"].GetString());
      return new Material{
        .name = name,
        .location = root_,
        .albedo = ParseMaterialComponent("albedo"),
        .ao = ParseMaterialComponent("ao"),
        .height = ParseMaterialComponent("height"),
        .metallic = ParseMaterialComponent("metallic"),
        .normal = ParseMaterialComponent("normal"),
        .roughness = ParseMaterialComponent("roughness"),
      };
    }

    Material* Material::LoadFrom(const std::string& filename) {
      return JsonMaterialLoader::Load(filename);
    }
  }

  MaterialRef GetMaterial(const resource::Token& token) {
    const auto material = material::JsonMaterialLoader::Load(token.location);
    const auto ptr = resource::Pointer(token.tag, (uword) material);
    return MaterialRef(ptr);
  }
}