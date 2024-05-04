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
    public:
      explicit MaterialDocument(const ConstObject& doc):
        doc_(doc) {
      }
      explicit MaterialDocument(const Value& doc): //TODO: assert doc is object
        MaterialDocument(doc.GetObject()) {
      }
      explicit MaterialDocument(const Document& doc):
        MaterialDocument(doc.GetObject()) { //TODO: assert that doc is object
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

#endif //MCC_MATERIAL_DOCUMENT_H