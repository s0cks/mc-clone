#ifndef MCC_SHADER_JSON_H
#define MCC_SHADER_JSON_H

#include "mcc/json.h"
#include "mcc/json_spec.h"

namespace mcc::json {
  template<const bool IsConst>
  class GenericShaderObject {
  private:
    typedef GenericObject<IsConst, Document::ValueType> ObjectType;
    static constexpr const auto kSourcePropertyName = "source";
  protected:
    ObjectType value_;

    std::optional<const Value*> GetProperty(const char* name) const {
      if(!value_.HasMember(name))
        return std::nullopt;
      return { &value_[name] };
    }
  public:
    GenericShaderObject() = default;
    explicit GenericShaderObject(const ObjectType& value):
      value_(value) {
    }
    GenericShaderObject(const GenericShaderObject<IsConst>& rhs) = default;
    ~GenericShaderObject() = default;

    std::optional<const Value*> GetSourceProperty() const {
      return GetProperty(kSourcePropertyName);
    }

    std::string GetSource() const {
      const auto source = GetSourceProperty();
      return source ? std::string((*source)->GetString(), (*source)->GetStringLength()) : std::string();
    }

    GenericShaderObject<IsConst>& operator=(const GenericShaderObject<IsConst>& rhs) = default;
  };
  typedef GenericShaderObject<true> ConstShaderObject;
  typedef GenericShaderObject<false> ShaderObject;
}

#endif //MCC_SHADER_JSON_H