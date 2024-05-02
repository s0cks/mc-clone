#ifndef MCC_MATERIAL_COMPONENT_H
#define MCC_MATERIAL_COMPONENT_H

#include <set>
#include <cstdint>
#include <ostream>

#include "mcc/texture/texture.h"

namespace mcc::material {
#define FOR_EACH_MATERIAL_COMPONENT(V)        \
  V(Albedo)                                   \
  V(Ao)                                       \
  V(Height)                                   \
  V(Metallic)                                 \
  V(Normal)                                   \
  V(Roughness)

  struct MaterialComponent {
    enum Type : uint8_t {
#define DEFINE_TYPE(Name) k##Name,
      FOR_EACH_MATERIAL_COMPONENT(DEFINE_TYPE)
#undef DEFINE_TYPE
    };
    
    friend std::ostream& operator<<(std::ostream& stream, const Type& rhs) {
      switch(rhs) {
#define DEFINE_TO_STRING(Name)        \
        case k##Name: return stream << #Name;
        FOR_EACH_MATERIAL_COMPONENT(DEFINE_TO_STRING)
#undef DEFINE_TO_STRING
        default: return stream << "Unknown MaterialComponent: " << static_cast<uint32_t>(rhs);
      }
    }

    struct TypeComparator {
      bool operator()(const MaterialComponent& lhs, const MaterialComponent& rhs) const {
        return lhs.type == rhs.type;
      }
    };

    Type type;
    Texture* texture;

    MaterialComponent() = default;
    MaterialComponent(const MaterialComponent& rhs) = default;
    ~MaterialComponent() = default;

    TextureId GetTextureId() const {
      return texture->GetTextureId();
    }

#define DEFINE_TYPE_CHECK(Name)       \
    bool Is##Name() const {           \
      return type == k##Name;         \
    }
    FOR_EACH_MATERIAL_COMPONENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK

    MaterialComponent& operator=(const MaterialComponent& rhs) = default;

    bool operator==(const MaterialComponent& rhs) const {
      return type == rhs.type
          && GetTextureId() == rhs.GetTextureId(); //TODO: use Equals()
    }

    bool operator!=(const MaterialComponent& rhs) const {
      return type != rhs.type
          || GetTextureId() != rhs.GetTextureId();
    }

    bool operator<(const MaterialComponent& rhs) const {
      return type < rhs.type;
    }

    bool operator>(const MaterialComponent& rhs) const {
      return type > rhs.type;
    }

    friend std::ostream& operator<<(std::ostream& stream, const MaterialComponent& rhs) {
      stream << "MaterialComponent(";
      stream << "type=" << rhs.type << ", ";
      stream << "texture=" << rhs.texture->GetTextureId(); //TODO: use ToString()
      stream << ")";
      return stream;
    }
  };

  typedef std::set<MaterialComponent, MaterialComponent::TypeComparator> MaterialComponentSet;
}

#endif //MCC_MATERIAL_COMPONENT_H