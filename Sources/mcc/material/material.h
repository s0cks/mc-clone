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

    static inline rx::observable<MaterialEvent*>
    OnMaterialEvent(const std::string& name) {
      return OnMaterialEvent()
        .filter([name](MaterialEvent* event) {
          return event
              && EqualsIgnoreCase(event->GetMaterialName(), name);
        });
    }

#define DEFINE_ON_MATERIAL_EVENT(Name)                        \
    static inline rx::observable<Name##Event*>                \
    On##Name##Event() {                                       \
      return OnMaterialEvent()                                \
        .filter(Name##Event::Filter)                          \
        .map(Name##Event::Cast);                              \
    }                                                         \
    static inline rx::observable<Name##Event*>                \
    On##Name##Event(const std::string& name) {                \
      return OnMaterialEvent()                                \
        .filter(Name##Event::FilterByName(name))              \
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
               const MaterialComponentSet& components);

      static void Publish(MaterialEvent* event);

      template<class E, typename... Args>
      static inline void
      Publish(Args... args) {
        E event(args...);
        return Publish((MaterialEvent*) &event);
      }
    public:
      virtual ~Material();

      const std::string& GetName() const {
        return name_;
      }

      const MaterialComponentSet& GetComponents() const {
        return components_;
      }

      inline rx::observable<MaterialEvent*> OnEvent() const {
        return OnMaterialEvent(GetName());
      }
#define DEFINE_ON_MATERIAL_EVENT(Name)                                  \
      inline rx::observable<Name##Event*> On##Name##Event() const {     \
        return material::On##Name##Event(GetName());                    \
      }
      FOR_EACH_MATERIAL_EVENT(DEFINE_ON_MATERIAL_EVENT)
#undef DEFINE_ON_MATERIAL_EVENT

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