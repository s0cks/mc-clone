#ifndef MCC_MATERIAL_LOADER_H
#define MCC_MATERIAL_LOADER_H

#include "mcc/rx.h"
#include "mcc/material/material_component.h"

namespace mcc::material {
  class Material;
  class MaterialLoader {
  protected:
    MaterialLoader() = default;
    virtual std::string GetMaterialName() const = 0;
    virtual rx::observable<MaterialComponent> GetMaterialComponents() const = 0;
  public:
    virtual ~MaterialLoader() = default;
    virtual Material* LoadMaterial() const;
  };
}

#endif //MCC_MATERIAL_LOADER_H