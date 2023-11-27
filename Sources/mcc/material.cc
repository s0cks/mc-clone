#include "mcc/material.h"

namespace mcc {
  std::vector<std::string> all_materials_;

  namespace material {
    Material* JsonMaterialLoader::LoadMaterial() {
      if(!doc_.HasMember("name")) {
        DLOG(ERROR) << "no 'name' field found in material document.";
        return nullptr;
      }
      const auto name = std::string(doc_["name"].GetString());
      const auto material = new Material{
        .name = name,
        .location = root_,
        .albedo = ParseMaterialComponent("albedo"),
        .ao = ParseMaterialComponent("ao"),
        .height = ParseMaterialComponent("height"),
        .metallic = ParseMaterialComponent("metallic"),
        .normal = ParseMaterialComponent("normal"),
        .roughness = ParseMaterialComponent("roughness"),
      };
      return material;
    }

    Material* Material::LoadFrom(const std::string& filename) {
      return JsonMaterialLoader::Load(filename);
    }
  }

  bool RegisterMaterial(const std::string& name) {
    all_materials_.push_back(name);
    return true;
  }

  uint64_t GetNumberOfMaterials() {
    return all_materials_.size();
  }

  bool VisitAllMaterials(std::function<bool(const std::string&)>vis) {
    for(const auto& material : all_materials_) {
      if(!vis(material))
        return false;
    }
    return true;
  }

  MaterialRef GetMaterial(const resource::Token& token) {
    const auto material = material::JsonMaterialLoader::Load(token.location);
    return MaterialRef(material);
  }
}