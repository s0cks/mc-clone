#include "mcc/material/material.h"
#include "mcc/texture/texture.h"
#include "mcc/texture/texture_loader.h"

namespace mcc {
  std::vector<std::string> all_materials_;

  namespace material {
    TextureRef JsonMaterialLoader::ParseMaterialComponent(const char* name) {
      if(!doc_.HasMember(name))
        return TextureRef();
      const auto& value = doc_[name];
      if(value.IsBool()) {
        if(!value.GetBool()) // not enabled
          return TextureRef();
        const auto filename = root_ + "/" + name + ".png";
        DLOG(INFO) << "loading material " << name << " texture from: " << filename;
        texture::PngFileLoader loader(filename);
        return loader.Load();
      } else if(value.IsString()) {
        const auto filename = root_ + "/" + value.GetString() + ".png";
        DLOG(INFO) << "loading material " << name << " texture from: " << filename;
        texture::PngFileLoader loader(filename);
        return loader.Load();
      }
      DLOG(INFO) << "cannot determine material component '" << name << "' from json value";
      return TextureRef();
    }

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

  MaterialRef GetMaterial(const uri::Uri& uri) {
    MCC_ASSERT(uri.scheme == "material");
    NOT_IMPLEMENTED(ERROR);
    return MaterialRef();
  }
}