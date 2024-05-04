#ifndef MCC_MATERIAL_LOADER_DIR_H
#define MCC_MATERIAL_LOADER_DIR_H

#include "mcc/material/material_loader.h"

namespace mcc::material {
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

#endif //MCC_MATERIAL_LOADER_DIR_H