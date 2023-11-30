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
#include "mcc/pipeline.h"

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

      ~Material() = default;

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

      TextureRef ParseMaterialComponent(const char* name);
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
        if(!json::ParseJson(root + "/material.json", doc))
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
  }

  using resource::MaterialRef;

  bool RegisterMaterial(const std::string& name);
  uint64_t GetNumberOfMaterials();
  bool VisitAllMaterials(std::function<bool(const std::string&)> vis);
  MaterialRef GetMaterial(const uri::Uri& uri);

  static inline MaterialRef
  GetMaterial(const uri::basic_uri& uri) {
    return GetMaterial(uri::Uri(uri));
  }
  
  class ApplyMaterialPipeline : public Pipeline {
  protected:
    MaterialRef material_;
  public:
   explicit ApplyMaterialPipeline(MaterialRef material):
      Pipeline(),
      material_(material) {
    }
    ~ApplyMaterialPipeline() override = default;

    inline MaterialRef material() const {
      return material_;
    }

    void Render() override {
      if(!material_.valid())
        return;
        
      RenderChildren();
      material_->Bind();
    }
  };
}

#endif //MCC_MATERIAL_H