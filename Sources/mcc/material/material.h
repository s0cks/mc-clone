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

    class Material {
    public:
      static constexpr const auto kAlbedoTextureSlot = 0;
      static constexpr const auto kAoTextureSlot = 1;
      static constexpr const auto kHeightTextureSlot = 2;
      static constexpr const auto kMetallicTextureSlot = 3;
      static constexpr const auto kNormalTextureSlot = 4;
      static constexpr const auto kRoughnessTextureSlot = 5;
    protected:
      std::string name_;
      TextureRef albedo_;
      TextureRef ao_;
      TextureRef height_;
      TextureRef metallic_;
      TextureRef normal_;
      TextureRef roughness_;
    public:
      Material(const std::string& name,
               TextureRef albedo,
               TextureRef ao,
               TextureRef height,
               TextureRef metallic,
               TextureRef normal,
               TextureRef roughness):
        name_(name),
        albedo_(albedo),
        ao_(ao),
        height_(height),
        metallic_(metallic),
        normal_(normal),
        roughness_(roughness) {
      }
      virtual ~Material() = default;

      std::string name() const {
        return name_;
      }

      TextureRef albedo() const {
        return albedo_;
      }

      TextureRef ao() const {
        return ao_;
      }

      TextureRef height() const {
        return height_;
      }

      TextureRef metallic() const {
        return metallic_;
      }

      TextureRef normal() const {
        return normal_;
      }

      TextureRef roughness() const {
        return roughness_;
      }

      virtual void Bind() const {
        albedo_->Bind(kAlbedoTextureSlot);
        ao_->Bind(kAoTextureSlot);
        height_->Bind(kHeightTextureSlot);
        metallic_->Bind(kMetallicTextureSlot);
        normal_->Bind(kNormalTextureSlot);
        roughness_->Bind(kRoughnessTextureSlot);
      }
    public:
      static Material* LoadFrom(const std::string& filename);
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