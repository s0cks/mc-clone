#include <optional>
#include <fmt/format.h>
#include <unordered_map>
#include <unordered_set>

#include "mcc/spec.h"
#include "mcc/texture/texture.h"
#include "mcc/material/material.h"
#include "mcc/material/material_spec.h"
#include "mcc/material/material_builder.h"
#include "mcc/material/material_document.h"

namespace mcc {
  namespace material {
    static rx::subject<MaterialEvent*> events_;

    rx::observable<MaterialEvent*> OnMaterialEvent() {
      return events_.get_observable();
    }

    void Material::Publish(MaterialEvent* event) {
      MCC_ASSERT(event);
      const auto& subscriber = events_.get_subscriber();
      subscriber.on_next(event);
    }

    Material::Material(const std::string& name,
                       const MaterialComponentSet& components):
      name_(name),
      components_(components) {
      MCC_ASSERT(!name.empty());
      MCC_ASSERT(!components.empty());
      Publish<MaterialCreatedEvent>(this);
    }

    Material::~Material() {
      Publish<MaterialDestroyedEvent>(this);
    }

    std::string Material::ToString() const {
      std::stringstream ss;
      ss << "Material(";
      ss << "name=" << GetName();
      ss << ")";
      return ss.str();
    }

    static inline std::string
    CreateMaterialName(const uri::Uri& uri) {
      const auto& path = uri.path;
      const auto slashpos = path.find_last_of('/');
      if(slashpos == std::string::npos)
        return path;
      return path.substr(slashpos + 1);
    }

    static inline std::string
    GetMaterialRoot(const uri::Uri& uri) {
      const auto& path = uri.path;
      const auto slashpos = path.find_last_of('/');
      if(slashpos == std::string::npos)
        return path;
      return path.substr(0, path.size() - (path.size() - slashpos));
    }

    static inline void
    ResolveMaterialComponents(MaterialComponentSet& components, const std::string& root_path) {
      DLOG(INFO) << "resolving material components in " << root_path << ".....";
      NOT_IMPLEMENTED(FATAL); //TODO: implement
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

    static inline Material*
    LoadMaterialFromJsonFile(const uri::Uri& uri) {
      MCC_ASSERT(uri.HasScheme("file"));
      MCC_ASSERT(uri.HasExtension(".json"));
      MCC_ASSERT(FileExists(uri));
      DLOG(INFO) << "loading material from json " << uri << "....";

      json::Document doc;
      if(!json::ParseJson(uri, doc)) {
        LOG(ERROR) << "failed to parse material json from: " << uri;
        return nullptr;
      }

      json::SpecDocument spec(doc);
      const auto n = GetMaterialName(spec);
      const auto name = n ? n.value() : CreateMaterialName(uri);
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

    static inline Material*
    LoadMaterialFromJsonFile(const uri::basic_uri& uri) {
      return LoadMaterialFromJsonFile(uri::Uri(uri));
    }

    Material* Material::New(const uri::Uri& uri) {
      MCC_ASSERT(uri.HasScheme("material"));
      const auto base_path = fmt::format("{0:s}/materials", FLAGS_resources);
      const auto base_uri_path = fmt::format("{0:s}/{1:s}", base_path, uri.path);
      if(uri.HasExtension(".json")) {
        if(!FileExists(base_uri_path)) {
          LOG(ERROR) << "cannot find Material spec file: " << base_uri_path;
          return nullptr;
        }
        const auto new_uri = fmt::format("file://{0:s}", base_uri_path);
        return LoadMaterialFromJsonFile(new_uri);
      }

      // check for .json file
      {
        const auto json_path = fmt::format("{0:s}.json", base_uri_path);
        if(FileExists(json_path))
          return LoadMaterialFromJsonFile(json_path);
      }

      const auto name = CreateMaterialName(uri);
      const auto root_path = GetMaterialRoot(uri);
      MaterialComponentSet components;
      ResolveMaterialComponents(components, root_path);
      if(components.empty()) {
        LOG(ERROR) << "failed to find components for material " << uri << " in " << root_path;
        return nullptr;
      }
      DLOG(INFO) << "resolved " << components.size() << " material components in " << root_path << ".";
      return New(name, components);
    }
  }
}