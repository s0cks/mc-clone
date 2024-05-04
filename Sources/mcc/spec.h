#ifndef MCC_SPEC_H
#define MCC_SPEC_H

#include <memory>
#include "mcc/json.h"

namespace mcc {
  class Spec {
  protected:
    Spec() = default;
  public:
    virtual ~Spec() = default;
    virtual const char* GetName() const = 0;
  };

#define DEFINE_SPEC(Name)                                  \
  public:                                                  \
    const char* GetName() const override { return #Name; }

#define DECLARE_SPEC_PROPERTY(Name, Type) \
  virtual std::optional<Type> Get##Name() const = 0;

  class SpecDocument {
    DEFINE_NON_COPYABLE_TYPE(SpecDocument);
  public:
    static constexpr const auto kNameProperty = "name";
    static constexpr const auto kTypeProperty = "type";
    static constexpr const auto kSpecProperty = "spec";
  private:
    uri::Uri uri_;
    json::Document doc_;
  public:
    explicit SpecDocument(const uri::Uri& uri):
      uri_(uri),
      doc_() {
      MCC_ASSERT(uri.HasScheme("file"));
      MCC_ASSERT(uri.HasExtension(".json"));
      LOG_IF(FATAL, !json::ParseJson(uri, doc_)) << "failed to parse SpecDocument from: " << uri;
    }
    virtual ~SpecDocument() = default;

    const uri::Uri& GetUri() const {
      return uri_;
    }

    const json::Value& GetNameProperty() const {
      return doc_[kNameProperty];
    }

    virtual std::string GetName() const {
      const auto& name = GetNameProperty();
      MCC_ASSERT(name.IsString());
      return std::string(name.GetString(), name.GetStringLength());
    }

    const json::Value& GetTypeProperty() const {
      return doc_[kTypeProperty];
    }

    virtual std::string GetType() const {
      const auto& type = GetTypeProperty();
      MCC_ASSERT(type.IsString());
      return std::string(type.GetString(), type.GetStringLength());
    }

    const json::Value& GetSpecProperty() const {
      return doc_[kSpecProperty];
    }
  };
}

#endif //MCC_SPEC_H