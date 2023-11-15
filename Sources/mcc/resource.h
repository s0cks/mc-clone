#ifndef MCC_RESOURCE_H
#define MCC_RESOURCE_H

#include "mcc/material.h"
#include "mcc/shader/shader.h"
#include "mcc/texture/texture.h"
#include "mcc/resource_registry.h"

namespace mcc {
  namespace resource {
    typedef Reference<material::Material> MaterialRef;
    typedef Reference<shader::Shader> ShaderRef;
    typedef Reference<Texture> TextureRef;
  }

  template<typename T>
  static inline resource::Reference<T>
  GetResource(const resource::Tag& tag) {
    return resource::Registry::Get<T>(tag);
  }

  static inline resource::MaterialRef
  GetMaterial(const std::string& name) {
    return GetResource<material::Material>(resource::Tag::Material(name));
  }

  static inline resource::ShaderRef
  GetShader(const std::string& name) {
    return GetResource<shader::Shader>(resource::Tag::Shader(name));
  }

  static inline resource::TextureRef
  GetTexture(const std::string& name) {
    return GetResource<Texture>(resource::Tag::Texture(name));
  }
}

#endif //MCC_RESOURCE_H