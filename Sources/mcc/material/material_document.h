#ifndef MCC_MATERIAL_DOCUMENT_H
#define MCC_MATERIAL_DOCUMENT_H

#include <optional>

#include "mcc/uri.h"
#include "mcc/json.h"
#include "mcc/common.h"

namespace mcc {
  namespace json {
#define FOR_EACH_MATERIAL_DOCUMENT_PROPERTY(V)                \
  V(Albedo, albedo)                                           \
  V(Ao, ao)                                                   \
  V(Height, height)                                           \
  V(Metallic, metallic)                                       \
  V(Normal, normal)                                           \
  V(Roughness, roughness)

    class MaterialDocument {
    public:
#define DEFINE_PROPERTY(Name, value)                          \
      class Name##Property {                                  \
        friend class MaterialDocument;                        \
        static constexpr const auto kName = #value;           \
        static inline bool                                    \
        Has(const ConstObject& doc) {                         \
          return doc.HasMember(kName);                        \
        }                                                     \
        static inline const Value&                            \
        Get(const ConstObject& doc) {                         \
          MCC_ASSERT(Has(doc));                               \
          return doc[kName];                                  \
        }                                                     \
      };
      FOR_EACH_MATERIAL_DOCUMENT_PROPERTY(DEFINE_PROPERTY)
    protected:
      const ConstObject& doc_;

      inline const ConstObject& doc() const {
        return doc_;
      }
    private:
      static inline const ConstObject&
      ToConstObject(const Value& doc) {
        MCC_ASSERT(doc.IsObject());
        return doc.GetObject();
      }

      static inline const ConstObject&
      ToConstObject(const Document& doc) {
        MCC_ASSERT(doc.IsObject());
        return doc.GetObject();
      }
    public:
      explicit MaterialDocument(const ConstObject& doc):
        doc_(doc) {
      }
      explicit MaterialDocument(const Value& doc):
        MaterialDocument(ToConstObject(doc)) {
      }
      explicit MaterialDocument(const Document& doc):
        MaterialDocument(ToConstObject(doc)) {  
      }
      ~MaterialDocument() = default;

#define DEFINE_PROPERTY_FUNCTIONS(Name, _)                  \
      inline const Value& Get##Name##Property() const {     \
        return Name##Property::Get(doc());                  \
      }                                                     \
      inline bool Has##Name##Property() const {             \
        return Name##Property::Has(doc());                  \
      }
      FOR_EACH_MATERIAL_DOCUMENT_PROPERTY(DEFINE_PROPERTY_FUNCTIONS)
#undef DEFINE_PROPERTY_FUNCTIONS
    };
#undef DEFINE_PROPERTY
  }
}

namespace mcc::material {
  class MaterialDocument {
    typedef const json::Value& Property;
  public:
    static constexpr const auto kAlbedoPropertyName = "albedo";
    static constexpr const auto kAoPropertyName = "ao";
    static constexpr const auto kHeightPropertyName = "height";
    static constexpr const auto kMetallicPropertyName = "metallic";
    static constexpr const auto kNormalPropertyName = "normal";
    static constexpr const auto kRoughnessPropertyName = "roughness";
  protected:
    uri::Uri uri_;
    json::Document doc_;
  public:
    explicit MaterialDocument(const uri::Uri& uri);
    virtual ~MaterialDocument() = default;

    uri::Uri GetUri() const {
      return uri_;
    }

    Property GetAlbedoProperty() const {
      return doc_[kAlbedoPropertyName];
    }

    Property GetAoProperty() const {
      return doc_[kAoPropertyName];
    }

    Property GetHeightProperty() const {
      return doc_[kHeightPropertyName];
    }

    Property GetMetallicProperty() const {
      return doc_[kMetallicPropertyName];
    }

    Property GetNormalProperty() const {
      return doc_[kNormalPropertyName];
    }

    Property GetRoughnessProperty() const {
      return doc_[kRoughnessPropertyName];
    }
  };
}

#endif //MCC_MATERIAL_DOCUMENT_H