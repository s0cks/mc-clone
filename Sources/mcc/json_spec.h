#ifndef MCC_JSON_SPEC_H
#define MCC_JSON_SPEC_H

#include "mcc/json.h"

namespace mcc::json {
  class SpecObject {
  protected:
    const json::Object& value_;

    explicit SpecObject(const json::Object& value):
      value_(value) {
    }

    inline const json::Object&
    value() const {
      return value_;
    }
  public:
    virtual ~SpecObject() = default;
  };

  class SpecDocument : public SpecObject {
  public:
    static constexpr const auto kTypePropertyName = "type";
    static constexpr const auto kNamePropertyName = "name";
    static constexpr const auto kSpecPropertyName = "spec";
  public:
    explicit SpecDocument(const json::Object& value):
      SpecObject(value) {
    }
    ~SpecDocument() override = default;

    bool HasTypeProperty() const {
      return value_.HasMember(kTypePropertyName);
    }

    const Value& GetTypeProperty() const {
      MCC_ASSERT(HasTypeProperty());
      return value_[kTypePropertyName];
    }

    bool HasNameProperty() const {
      return value_.HasMember(kNamePropertyName);
    }

    const Value& GetNameProperty() const {
      return value_[kNamePropertyName];
    }

    bool HasSpecProperty() const {
      return value_.HasMember(kSpecPropertyName);
    }

    const Value& GetSpecProperty() const {
      MCC_ASSERT(HasSpecProperty());
      return value_[kSpecPropertyName];
    }
  };
}

#endif //MCC_JSON_SPEC_H