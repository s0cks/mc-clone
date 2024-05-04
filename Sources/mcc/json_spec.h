#ifndef MCC_JSON_SPEC_H
#define MCC_JSON_SPEC_H

#include "mcc/json.h"

namespace mcc::json {
  class SpecObject {
  protected:
    const ConstObject& value_;

    explicit SpecObject(const ConstObject& value):
      value_(value) {
    }

    inline const ConstObject&
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
    explicit SpecDocument(const ConstObject& value):
      SpecObject(value) {
    }
    explicit SpecDocument(const Document& value):
      SpecDocument(value.GetObject()) {
    }
    ~SpecDocument() override = default;

    bool HasTypeProperty() const {
      return value().HasMember(kTypePropertyName);
    }

    const Value& GetTypeProperty() const {
      MCC_ASSERT(HasTypeProperty());
      return value()[kTypePropertyName];
    }

    bool HasNameProperty() const {
      return value().HasMember(kNamePropertyName);
    }

    const Value& GetNameProperty() const {
      return value()[kNamePropertyName];
    }

    bool HasSpecProperty() const {
      return value().HasMember(kSpecPropertyName);
    }

    const Value& GetSpecProperty() const {
      MCC_ASSERT(HasSpecProperty());
      return value()[kSpecPropertyName];
    }
  };
}

#endif //MCC_JSON_SPEC_H