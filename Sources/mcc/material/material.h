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

#include "mcc/material/material_events.h"
#include "mcc/material/material_registry.h"

namespace mcc {
  namespace material {
    const MaterialRegistry& GetRegistry();
    rx::observable<MaterialEvent*> OnMaterialEvent();

#define DEFINE_ON_MATERIAL_EVENT(Name)                        \
    static inline rx::observable<Name##Event*>                \
    On##Name##Event() {                                       \
      return OnMaterialEvent()                                \
        .filter(Name##Event::Filter)                          \
        .map(Name##Event::Cast);                              \
    }
    FOR_EACH_MATERIAL_EVENT(DEFINE_ON_MATERIAL_EVENT)
#undef DEFINE_ON_MATERIAL_EVENT

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

      const std::string& GetName() const {
        return name_;
      }

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

      virtual std::string ToString() const;
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

  MaterialRef GetMaterial(const uri::Uri& uri);

  static inline MaterialRef
  GetMaterial(const uri::basic_uri& uri) {
    return GetMaterial(uri::Uri(uri));
  }
  
  class ApplyMaterialPipeline : public Pipeline {
  protected:
    MaterialRef material_;

    bool Apply() override {
      if(!material_.valid())
        return false;
      material_->Bind();
      return true;
    }
  public:
   explicit ApplyMaterialPipeline(MaterialRef material):
      Pipeline(),
      material_(material) {
    }
    ~ApplyMaterialPipeline() override = default;

    inline MaterialRef material() const {
      return material_;
    }
  };
}

#endif //MCC_MATERIAL_H