#ifndef MCC_MATERIAL_SPEC_LOADER_H
#define MCC_MATERIAL_SPEC_LOADER_H

#include "mcc/rx.h"
#include "mcc/material/material_spec.h"
#include "mcc/material/material_loader.h"

namespace mcc::material {
  class Material;
  class MaterialSpecLoader : public MaterialLoader {
  protected:
    const uri::Uri& uri_;
  public:
    explicit MaterialSpecLoader(const uri::Uri& uri):
      uri_(uri) {
      MCC_ASSERT(uri.HasScheme("file"));
      MCC_ASSERT(uri.HasExtension(".json"));
      MCC_ASSERT(FileExists(uri));
    }
    ~MaterialSpecLoader() = default;
    Material* LoadMaterial() const override;
  public:
    static inline Material*
    Load(const uri::Uri& uri) {
      MaterialSpecLoader loader(uri);
      return loader.LoadMaterial();
    }

    static inline Material*
    Load(const uri::basic_uri& uri) {
      return Load(uri::Uri(uri));
    }

    static inline rx::observable<Material*>
    LoadAsync(const uri::Uri& uri) {
      return rx::observable<>::create<Material*>([uri](rx::subscriber<Material*> s) {
        MaterialSpecLoader loader(uri);
        const auto material = loader.LoadMaterial();
        if(!material)
          return s.on_error(rx::util::make_error_ptr(std::runtime_error("Failed to create material.")));
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