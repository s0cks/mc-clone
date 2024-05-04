#include <fmt/format.h>
#include <unordered_map>
#include <unordered_set>

#include "mcc/texture/texture.h"
#include "mcc/material/material.h"
#include "mcc/material/material_spec.h"

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
  }

  MaterialRef GetMaterial(const uri::Uri& uri) {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return MaterialRef();
  }
}