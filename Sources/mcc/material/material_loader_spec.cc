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

  Material* MaterialSpecLoader::LoadMaterial() const {
    DLOG(INFO) << "loading material from json " << uri_ << "....";
    json::Document doc;
    if(!json::ParseJson(uri_, doc)) {
      LOG(ERROR) << "failed to parse material json from: " << uri_;
      return nullptr;
    }

    json::SpecDocument spec(doc);
    const auto n = GetMaterialName(spec);
    const auto name = n ? n.value() : CreateMaterialName(uri_);
    const auto type = GetMaterialType(spec);
    MCC_ASSERT(type);
    MCC_ASSERT(EqualsIgnoreCase(*type, "material"));
    MaterialComponentSet components;

    json::MaterialDocument material(spec.GetSpecProperty());
    if(material.HasAlbedoProperty()) {
      components.insert(MaterialComponent {
        .type = MaterialComponent::kAlbedo,
        .texture = nullptr, //TODO: implement
      });
    }

    if(material.HasAoProperty()) {
      components.insert(MaterialComponent {
        .type = MaterialComponent::kAo,
        .texture = nullptr, //TODO: implement
      });
    }

    return Material::New(name, components);
  }
}