#ifndef MCC_MATERIAL_LOADER_H
#define MCC_MATERIAL_LOADER_H

#include "mcc/rx.h"
#include "mcc/material/material_component.h"

namespace mcc::material {
  class Material;
  class MaterialLoader {
  protected:
    MaterialLoader() = default;
  public:
    virtual ~MaterialLoader() = default;
    virtual Material* LoadMaterial() const = 0;
  };

  class MaterialDirectoryLoader : public MaterialLoader {
  protected:
    std::string name_;
    std::string root_;
    MaterialComponentSet components_;
  public:
    MaterialDirectoryLoader(const std::string& name, const std::string& root):
      MaterialLoader(),
      name_(name),
      root_(root),
      components_() {
    }
    ~MaterialDirectoryLoader() override = default;

    const std::string& GetName() const {
      return name_;
    }

    const std::string& GetRoot() const {
      return root_;
    }

    const MaterialComponentSet& GetComponents() const {
      return components_;
    }

    Material* LoadMaterial() const override;
  public:
    static inline Material*
    LoadAny(const std::string& name, const std::string& root) {
      MaterialDirectoryLoader loader(name, root);
      return loader.LoadMaterial();
    }
  };
}

#endif //MCC_MATERIAL_LOADER_H