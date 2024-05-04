#ifndef MCC_MATERIAL_H
#define MCC_MATERIAL_H

#include <fstream>
#include <memory>

#include "mcc/gfx.h"
#include "mcc/flags.h"
#include "mcc/buffer.h"

#include "mcc/json.h"
#include "mcc/texture/texture.h"
#include "mcc/pipeline.h"

#include "mcc/material/material_events.h"
#include "mcc/material/material_registry.h"
#include "mcc/material/material_component.h"

namespace mcc {
  namespace material {
    const MaterialRegistry& GetRegistry();
    rx::observable<MaterialEvent*> OnMaterialEvent();

#define DEFINE_ON_MATERIAL_EVENT(Name)                        \
    static inline rx::observable<Name##Event*>                \
    On##Name##Event() {                                       \
      return OnMaterialEvent()                                \
        .filter(Name##Event::Filter)                          \
        .map(Name##Event::Cast);                              \
    }
    FOR_EACH_MATERIAL_EVENT(DEFINE_ON_MATERIAL_EVENT)
#undef DEFINE_ON_MATERIAL_EVENT

    class Material {
      friend class MaterialBuilder;
    protected:
      std::string name_;
      MaterialComponentSet components_;

      Material(const std::string& name,
               const MaterialComponentSet& components):
        name_(name),
        components_(components) {
        MCC_ASSERT(!name.empty());
        MCC_ASSERT(!components.empty());
      }
    public:
      virtual ~Material() = default;

      const std::string& GetName() const {
        return name_;
      }

      const MaterialComponentSet& GetComponents() const {
        return components_;
      }

      rx::observable<MaterialEvent*> OnEvent() const {
        return OnMaterialEvent()
          .filter([this](MaterialEvent* event) {
            return event
                && EqualsIgnoreCase(GetName(), event->GetMaterialName());
          });
      }
#define DEFINE_ON_MATERIAL_EVENT(Name)                        \
      rx::observable<

      virtual std::string ToString() const;
    };
  }

  using material::Material;

  namespace resource {
    typedef Reference<Material> MaterialRef;
  }

  using resource::MaterialRef;

  MaterialRef GetMaterial(const uri::Uri& uri);

  static inline MaterialRef
  GetMaterial(const uri::basic_uri& uri) {
    return GetMaterial(uri::Uri(uri));
  }
}

#endif //MCC_MATERIAL_H