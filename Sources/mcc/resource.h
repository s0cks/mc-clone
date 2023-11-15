#ifndef MCC_RESOURCE_H
#define MCC_RESOURCE_H

#include "mcc/material.h"
#include "mcc/resource_registry.h"

namespace mcc {
  template<typename T>
  static inline std::optional<resource::Reference<T>>
  GetResource(const resource::Tag& tag) {
    return resource::Registry::Get<T>(tag);
  }

  static inline std::optional<resource::Reference<material::Material>>
  GetMaterial(const std::string& name) {
    return GetResource<material::Material>(resource::Tag::Material(name));
  }
}

#endif //MCC_RESOURCE_H