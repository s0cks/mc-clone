#ifndef MCC_MATERIAL_SPEC_H
#define MCC_MATERIAL_SPEC_H

#include "mcc/json_spec.h"

namespace mcc::json {
  class MaterialSpec : public SpecObject {
  public:
    static constexpr const auto kAlbedoPropertyName = "albedo";
    static constexpr const auto kAoPropertyName = "ao";
    static constexpr const auto kHeightPropertyName = "height";
    static constexpr const auto kNormalPropertyName = "normal";
    static constexpr const auto kMetallicPropertyName = "metallic";
    static constexpr const auto kRoughnessPropertyName = "roughness";
  public:
    explicit MaterialSpec(const json::Object& value):
      SpecObject(value) {
    }
    ~MaterialSpec() override = default;

    bool HasAlbedoProperty() const {
      return value_.HasMember(kAlbedoPropertyName);
    }

    const Value& GetAlbedoProperty() const {
      MCC_ASSERT(HasAlbedoProperty());
      return value_[kAlbedoPropertyName];
    }

    bool HasAoProperty() const {
      return value_.HasMember(kAoPropertyName);
    }

    const Value& GetAoProperty() const {
      MCC_ASSERT(HasAoProperty());
      return value_[kAoPropertyName];
    }

    bool HasHeightProperty() const {
      return value_.HasMember(kHeightPropertyName);
    }

    const Value& GetHeightProperty() const {
      return value_[kHeightPropertyName];
    }
  };
}

#endif //MCC_MATERIAL_SPEC_H