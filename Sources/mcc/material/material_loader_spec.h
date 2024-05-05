#ifndef MCC_MATERIAL_SPEC_LOADER_H
#define MCC_MATERIAL_SPEC_LOADER_H

#include "mcc/rx.h"
#include "mcc/material/material_spec.h"
#include "mcc/material/material_loader.h"

namespace mcc::material {
  class Material;
  class MaterialSpecLoader : public MaterialLoader {
  protected:
    json::SpecDocument doc_;

    std::string GetMaterialName() const override;
    rx::observable<MaterialComponent> GetMaterialComponents() const override;
  public:
    explicit MaterialSpecLoader(const json::Object& doc):
      doc_(doc) {
    }
    ~MaterialSpecLoader() = default;
  public:
    static inline Material*
    Load(const uri::Uri& uri) {
      json::Document doc;
      if(!json::ParseJson(uri, doc)) {
        LOG(ERROR) << "failed to parse json document from: " << uri;
        return nullptr;
      }
      const auto spec = doc.GetObject();
      MaterialSpecLoader loader(spec);
      return loader.LoadMaterial();
    }

    static inline Material*
    Load(const uri::basic_uri& uri) {
      return Load(uri::Uri(uri));
    }

    static inline rx::observable<Material*>
    LoadAsync(const uri::Uri& uri) {
      return rx::observable<>::create<Material*>([uri](rx::subscriber<Material*> s) {
        json::Document doc;
        if(!json::ParseJson(uri, doc)) {
          const auto err = fmt::format("failed to parse json document from: {0:s}", (const std::string&) uri);
          return s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
        }
        const auto spec = doc.GetObject();
        MaterialSpecLoader loader(spec);
        const auto material = loader.LoadMaterial();
        if(!material) {
          const auto err = "failed to create material.";
          return s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
        }
        s.on_next(material);
        s.on_completed();
      });
    }

    static inline rx::observable<Material*>
    LoadAsync(const uri::basic_uri& uri) {
      return LoadAsync(uri::Uri(uri));
    }
  };
}

#endif //MCC_MATERIAL_SPEC_LOADER_H