#ifndef MCC_PROGRAM_JSON_H
#define MCC_PROGRAM_JSON_H

#include "mcc/json.h"

namespace mcc::json {
  template<const bool IsConst>
  class GenericProgramObject {
    typedef GenericObject<IsConst, Document::ValueType> ObjectType;

    static constexpr const auto kVertexPropertyName = "vertex";
    static constexpr const auto kFragmentPropertyName = "fragment";
    static constexpr const auto kGeometryPropertyName = "geometry";
    static constexpr const auto kTessEvalPropertyName = "tesseval";
    static constexpr const auto kTessControlPropertyName = "tesscontrol";
  protected:
    ObjectType value_;

    inline const ObjectType& value() const {
      return value_;
    }

    inline std::optional<const Value*> GetProperty(const char* name) const {
      if(!value().HasMember(name))
        return std::nullopt;
      return { &value()[name] };
    }
  public:
    explicit GenericProgramObject(const ObjectType& value):
      value_(value) {
    }
    ~GenericProgramObject() = default;

    std::optional<const Value*> GetVertexProperty() const {
      return GetProperty(kVertexPropertyName);
    }

    std::optional<const Value*> GetFragmentProperty() const {
      return GetProperty(kFragmentPropertyName);
    }

    std::optional<const Value*> GetGeometryProperty() const {
      return GetProperty(kGeometryPropertyName);
    }

    friend std::ostream& operator<<(std::ostream& stream, const GenericProgramObject<IsConst>& rhs) {
      stream << "json::Program(";
      stream << "...";
      {
        const auto prop = rhs.GetVertexProperty();
        if(prop) {
          if((*prop)->IsString()) {
            stream << ", vertex=" << std::string((*prop)->GetString(), (*prop)->GetStringLength());
          } else if((*prop)->IsObject()) {

          } else {
            stream << "vertex=Unknown Json Type.";
          }
        }
      }
      {
        const auto prop = rhs.GetFragmentProperty();
        if(prop) {
          if((*prop)->IsString()) {
            stream << ", fragment=" << std::string((*prop)->GetString(), (*prop)->GetStringLength());
          } else if((*prop)->IsObject()) {

          } else {
            stream << "fragment=Unknown Json Type.";
          }
        }
      }
      {
        const auto prop = rhs.GetGeometryProperty();
        if(prop) {
          if((*prop)->IsString()) {
            stream << ", geometry=" << std::string((*prop)->GetString(), (*prop)->GetStringLength());
          } else if((*prop)->IsObject()) {

          } else {
            stream << ", geometry=Unknown Json Type.";
          }
        }
      }
      stream << ")";
      return stream;
    }
  };
  typedef GenericProgramObject<true> ConstProgramObject;
  typedef GenericProgramObject<false> ProgramObject;
}

#endif //MCC_PROGRAM_JSON_H