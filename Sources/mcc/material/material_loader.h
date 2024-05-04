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
}

#endif //MCC_MATERIAL_LOADER_H