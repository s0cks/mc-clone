#ifndef MCC_MATERIAL_LOADER_H
#define MCC_MATERIAL_LOADER_H

#include "mcc/material/material.h"

namespace mcc::material {
  class MaterialLoader {
  protected:
    MaterialLoader() = default;
  public:
    virtual ~MaterialLoader() = default;
    virtual Material* Load() = 0;
  };

  class DefaultMaterialLoader : public MaterialLoader {
    static constexpr const auto kAlbedoTextureName = "albedo";
    static constexpr const auto kOcclusionTextureName = "ao";
    static constexpr const auto kHeightTextureName = "height";
    static constexpr const auto kMetallicTextureName = "metallic";
    static constexpr const auto kNormalTextureName = "normal";
    static constexpr const auto kRoughnessTextureName = "roughness";
  protected:
    std::string root_;
    std::string name_;

    TextureRef FindTexture(const std::string& name);

    inline TextureRef FindAlbedoTexture() {
      return FindTexture(kAlbedoTextureName);
    }

    inline TextureRef FindOcclusionTexture() {
      return FindTexture(kOcclusionTextureName);
    }

    inline TextureRef FindHeightTexture() {
      return FindTexture(kHeightTextureName);
    }

    inline TextureRef FindMetallicTexture() {
      return FindTexture(kMetallicTextureName);
    }

    inline TextureRef FindNormalTexture() {
      return FindTexture(kNormalTextureName);
    }

    inline TextureRef FindRoughnessTexture() {
      return FindTexture(kRoughnessTextureName);
    }

    static inline std::string
    GetMaterialName(const std::string& path) {
      auto name = path;
      if(EndsWith(name, "/"))
        name = name.substr(0, name.length() - 1);
      const auto slashpos = name.find_last_of('/');
      if(slashpos == std::string::npos)
        return name;
      name = name.substr(slashpos + 1);
      return name;
    }
  public:
    DefaultMaterialLoader(const std::string& root, const std::string& name):
      root_(root),
      name_(name) {  
    }
    DefaultMaterialLoader(const std::string& root):
      root_(root),
      name_(GetMaterialName(root)) {
    }
    ~DefaultMaterialLoader() override = default;
    
    Material* Load() override;
  };

  class JsonMaterialLoader : public MaterialLoader {
  private:
    std::string root_;
    json::Document& doc_;

    TextureRef ParseMaterialComponent(const char* name);
  public:
    explicit JsonMaterialLoader(const std::string& root,
                                json::Document& doc):
      MaterialLoader(),
      root_(root),
      doc_(doc) {
    }
    ~JsonMaterialLoader() override = default;
    Material* Load() override;
  public:
    static inline Material*
    LoadMaterial(const std::string& root) {
      json::Document doc;
      if(!json::ParseJson(root, doc))
        return nullptr;
      material::JsonMaterialLoader loader(root, doc);
      return loader.Load();
    }
  };
}

#endif //MCC_MATERIAL_LOADER_H