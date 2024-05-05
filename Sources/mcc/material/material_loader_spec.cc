#include "mcc/material/material_loader_spec.h"

#include "mcc/material/material.h"
#include "mcc/material/material_document.h"
#include "mcc/material/material_component.h"

namespace mcc::material {
  static inline std::string
  CreateMaterialName(const uri::Uri& uri) {
    const auto& path = uri.path;
    const auto slashpos = path.find_last_of('/');
    if(slashpos == std::string::npos)
      return path;
    return path.substr(slashpos + 1);
  }

  static inline std::optional<std::string>
  GetMaterialName(const json::SpecDocument& spec) {
    if(!spec.HasNameProperty())
      return std::nullopt;
    const auto& name = spec.GetNameProperty();
    MCC_ASSERT(name.IsString());
    return { std::string(name.GetString(), name.GetStringLength()) };
  }

  static inline std::optional<std::string>
  GetMaterialType(const json::SpecDocument& spec) {
    if(!spec.HasTypeProperty())
      return std::nullopt;
    const auto& type = spec.GetTypeProperty();
    MCC_ASSERT(type.IsString());
    return { std::string(type.GetString(), type.GetStringLength()) };
  }

  std::string MaterialSpecLoader::GetMaterialName() const {
    const auto& name = doc_.GetNameProperty();
    MCC_ASSERT(name.IsString());
    return std::string(name.GetString(), name.GetStringLength());
  }

  rx::observable<MaterialComponent> MaterialSpecLoader::GetMaterialComponents() const {
    return rx::observable<>::create<MaterialComponent>([this](rx::subscriber<MaterialComponent> s) {
      const auto& spec = doc_.GetSpecProperty();
      MCC_ASSERT(spec.IsObject());
      const auto spec_obj = spec.GetObject();
      json::MaterialDocument material(spec_obj);
      if(material.HasAlbedoProperty())
        s.on_next(MaterialComponent {
          .type = MaterialComponent::kAlbedo,
          .texture = nullptr, //TODO: implement
        });
      if(material.HasAoProperty())
        s.on_next(MaterialComponent {
          .type = MaterialComponent::kAo,
          .texture = nullptr, //TODO: implement
        });
      s.on_completed();
    });
  }
}