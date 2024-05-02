#ifndef MCC_MATERIAL_DOCUMENT_H
#define MCC_MATERIAL_DOCUMENT_H

#include "mcc/uri.h"
#include "mcc/json.h"
#include "mcc/common.h"

namespace mcc::material {
#define FOR_EACH_MATERIAL_DOCUMENT_PROPERTY(V)            \
  V(Name)

  class MaterialDocument {
  public:
#define DEFINE_PROPERTY(Name)                   \
    struct Name##Property {                     \
      friend class MaterialDocument;            \
    public:                                     \
      static constexpr const char*              \
      GetName() {                               \
        return #Name;                           \
      }                                         \
    private:                                    \
      static inline bool                        \
      Contains(const json::Document& doc) {     \
        return doc.HasMember(GetName());        \
      }                                         \
      static inline const json::Value&          \
      Get(const json::Document& doc) {          \
        MCC_ASSERT(Contains(doc));              \
        return doc[GetName()];                  \
      }                                         \
    };
    FOR_EACH_MATERIAL_DOCUMENT_PROPERTY(DEFINE_PROPERTY);
#undef DEFINE_PROPERTY
  protected:
    uri::Uri uri_;
    json::Document doc_;
  public:
    explicit MaterialDocument(const uri::Uri& uri);
    virtual ~MaterialDocument() = default;

    uri::Uri GetUri() const {
      return uri_;
    }

#define DEFINE_PROPERTY_FUNCTIONS(Name)                             \
    virtual bool Has##Name##Property() const {                      \
      return Name##Property::Contains(doc_);                        \
    }                                                               \
    virtual const json::Value& Get##Name##Property() const {        \
      return Name##Property::Get(doc_);                             \
    }
    FOR_EACH_MATERIAL_DOCUMENT_PROPERTY(DEFINE_PROPERTY_FUNCTIONS)
#undef DEFINE_PROPERTY_FUNCTIONS
  };
}

#endif //MCC_MATERIAL_DOCUMENT_H