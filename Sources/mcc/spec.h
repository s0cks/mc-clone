#ifndef MCC_SPEC_H
#define MCC_SPEC_H

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
    static constexpr const auto kDataProperty = "data";
  private:
    uri::Uri uri_;
    json::Document doc_;
  protected:
    explicit SpecDocument(const uri::Uri& uri);
  public:
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

    const json::Value& GetDataProperty() const {
      return doc_[kDataProperty];
    }
  };

  template<class T>
  class SpecDocumentTemplate : public SpecDocument {
  protected:
    SpecDocumentTemplate() = default;
  public:
    ~SpecDocumentTemplate() override = default;

    virtual T* GetData() const {
      return new T(GetDataProperty());
    }
  };
}

#endif //MCC_SPEC_H