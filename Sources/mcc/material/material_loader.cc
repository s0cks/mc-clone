#include "mcc/material/material_loader.h"

#include <fmt/format.h>
#include "mcc/texture/texture.h"

namespace mcc::material {
  static const std::vector<std::string> kValidImageExtension = {
    ".jpeg",
    ".jpg",
    ".png"
  };

  TextureRef DefaultMaterialLoader::FindTexture(const std::string& name) {
    DLOG(INFO) << "finding " << name << " texture for " << name_;
    TextureRef result = TextureRef();
    std::for_each(kValidImageExtension.begin(), kValidImageExtension.end(), [this,&name,&result](const std::string& extension) {
      const auto filename = fmt::format("{0:s}/{1:s}.{2:s}", root_, name, extension);
      if(FileExists(filename)) {
        DLOG(INFO) << "found " << filename;
        //TODO: load texture
        return false;
      }

      return true;
    });
    return result;
  }

  Material* DefaultMaterialLoader::Load() {
    const auto material = new Material(name_, 
      FindAlbedoTexture(),
      FindOcclusionTexture(),
      FindHeightTexture(),
      FindMetallicTexture(),
      FindNormalTexture(),
      FindRoughnessTexture());
    return material;
  }

  TextureRef JsonMaterialLoader::ParseMaterialComponent(const char* name) {
    if(!doc_.HasMember(name))
      return TextureRef();
    const auto& value = doc_[name];
    if(value.IsBool()) {
      if(!value.GetBool()) // not enabled
        return TextureRef();
      const auto filename = root_ + "/" + name + ".png";
      DLOG(INFO) << "loading material " << name << " texture from: " << filename;
      //TODO: implement
      // texture::PngFileLoader loader(filename);
      // return loader.Load();
      return TextureRef();
    } else if(value.IsString()) {
      const auto filename = root_ + "/" + value.GetString() + ".png";
      //TODO:
      // DLOG(INFO) << "loading material " << name << " texture from: " << filename;
      // texture::PngFileLoader loader(filename);
      // return loader.Load();
      return TextureRef();
    }
    DLOG(INFO) << "cannot determine material component '" << name << "' from json value";
    return TextureRef();
  }

  Material* JsonMaterialLoader::Load() {
    if(!doc_.HasMember("name")) {
      DLOG(ERROR) << "no 'name' field found in material document.";
      return nullptr;
    }
    const auto name = std::string(doc_["name"].GetString());
    return new Material(
      name,
      ParseMaterialComponent("albedo"),
      ParseMaterialComponent("ao"),
      ParseMaterialComponent("height"),
      ParseMaterialComponent("metallic"),
      ParseMaterialComponent("normal"),
      ParseMaterialComponent("roughness")
    );
  }
}