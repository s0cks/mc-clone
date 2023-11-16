#ifndef MCC_MATERIAL_H
#define MCC_MATERIAL_H

#include <fstream>
#include <memory>

#include "mcc/gfx.h"
#include "mcc/flags.h"
#include "mcc/buffer.h"
#include "mcc/materials_generated.h"

#include "mcc/json.h"
#include "mcc/texture/texture.h"

namespace mcc {
  namespace material {
    struct Material;
    typedef std::shared_ptr<Material> MaterialPtr;
    struct Material {
      std::string name;
      std::string location;
      TextureRef albedo;
      TextureRef ao;
      TextureRef height;
      TextureRef metallic;
      TextureRef normal;
      TextureRef roughness;

      ~Material();

      void Bind() const {
        albedo->Bind(0);
        ao->Bind(1);
        height->Bind(2);
        metallic->Bind(3);
        normal->Bind(4);
        roughness->Bind(5);
      }
    public:
      static Material* LoadFrom(const std::string& filename);
    };

    class MaterialLoader {
    protected:
      MaterialLoader() = default;
    public:
      virtual ~MaterialLoader() = default;
      virtual Material* LoadMaterial() = 0;
    };

    class JsonMaterialLoader : public MaterialLoader {
    private:
      std::string root_;
      json::Document& doc_;

      inline TextureRef ParseMaterialComponent(const char* name) {
        if(!doc_.HasMember(name))
          return TextureRef();
        const auto& value = doc_[name];
        if(value.IsBool()) {
          if(!value.GetBool()) // not enabled
            return TextureRef();
          const auto filename = root_ + "/" + name + ".png";
          DLOG(INFO) << "loading material " << name << " texture from: " << filename;
          return GetTextureFromFile(filename);
        } else if(value.IsString()) {
          const auto filename = root_ + "/" + value.GetString() + ".png";
          DLOG(INFO) << "loading material " << name << " texture from: " << filename;
          return GetTextureFromFile(filename);
        }
        DLOG(INFO) << "cannot determine material component '" << name << "' from json value";
        return TextureRef();
      }
    public:
      explicit JsonMaterialLoader(const std::string& root,
                                  json::Document& doc):
        MaterialLoader(),
        root_(root),
        doc_(doc) {
      }
      ~JsonMaterialLoader() override = default;
      Material* LoadMaterial() override;
    public:
      static inline Material*
      Load(const std::string& root) {
        json::Document doc;
        if(!ParseJson(root + "/material.json", doc))
          return nullptr;
        material::JsonMaterialLoader loader(root, doc);
        return loader.LoadMaterial();
      }
    };
  }

  using material::Material;
  using material::MaterialPtr;

  namespace resource {
    typedef Reference<Material> MaterialRef;

    static inline Tag
    NewMaterialTag(const std::string& name) {
      return Tag::Material(name);
    }
  }

  using resource::MaterialRef;

  bool RegisterMaterial(const std::string& name);
  uint64_t GetNumberOfMaterials();
  bool VisitAllMaterials(std::function<bool(const std::string&)> vis);
  MaterialRef GetMaterial(const resource::Token& token);

  static inline MaterialRef
  GetMaterial(const std::string& name) {
    const auto token = resource::Registry::Get(resource::NewMaterialTag(name));
    return GetMaterial(token);
  }
}

#endif //MCC_MATERIAL_H