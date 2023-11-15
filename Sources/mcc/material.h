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
      Texture albedo;
      Texture ao;
      Texture height;
      Texture metallic;
      Texture normal;
      Texture roughness;

      void Bind() const {
        albedo.Bind(0);
        ao.Bind(1);
        height.Bind(2);
        metallic.Bind(3);
        normal.Bind(4);
        roughness.Bind(5);
      }
    public:
      static MaterialPtr LoadFrom(const std::string& filename);
    };

    class MaterialLoader {
    protected:
      MaterialLoader() = default;
    public:
      virtual ~MaterialLoader() = default;
      virtual MaterialPtr LoadMaterial() = 0;
    };

    class JsonMaterialLoader : public MaterialLoader {
    private:
      std::string root_;
      json::Document& doc_;

      inline Texture ParseMaterialComponent(const char* name) {
        if(!doc_.HasMember(name))
          return kInvalidTextureId;
        const auto& value = doc_[name];
        if(value.IsBool()) {
          if(!value.GetBool()) // not enabled
            return kInvalidTextureId;
          const auto filename = root_ + "/" + name + ".png";
          DLOG(INFO) << "loading material " << name << " texture from: " << filename;
          return Texture::LoadFrom(filename);
        } else if(value.IsString()) {
          const auto filename = root_ + "/" + value.GetString() + ".png";
          DLOG(INFO) << "loading material " << name << " texture from: " << filename;
          return Texture::LoadFrom(filename);
        }
        DLOG(INFO) << "cannot determine material component '" << name << "' from json value";
        return kInvalidTextureId;
      }
    public:
      explicit JsonMaterialLoader(const std::string& root,
                                  json::Document& doc):
        MaterialLoader(),
        root_(root),
        doc_(doc) {
      }
      ~JsonMaterialLoader() override = default;
      MaterialPtr LoadMaterial() override;
    public:
      static inline MaterialPtr
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
}

#endif //MCC_MATERIAL_H