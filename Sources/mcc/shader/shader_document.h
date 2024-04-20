#ifndef MCC_SHADER_DOCUMENT_H
#define MCC_SHADER_DOCUMENT_H

#include "mcc/uri.h"
#include "mcc/json.h"
#include "mcc/shader/shader_type.h"

namespace mcc::shader {
#define FOR_EACH_SHADER_DOCUMENT_PROPERTY(V) \
  V(Name)                                    \
  V(Type)                                    \
  V(File)                                    \
  V(Code)

  class ShaderDocument {
#define DEFINE_SHADER_DOCUMENT_PROPERTY_NAME(Name)         \
    static constexpr const auto k##Name##Property = #Name;
    FOR_EACH_SHADER_DOCUMENT_PROPERTY(DEFINE_SHADER_DOCUMENT_PROPERTY_NAME)
#undef DEFINE_SHADER_DOCUMENT_PROPERTY
  protected:
    uri::Uri uri_;
    json::Document doc_;

    inline json::Document& doc() {
      return doc_;
    }

    inline const json::Document& doc() const {
      return doc_;
    }
  public:
    explicit ShaderDocument(const uri::Uri& uri):
      uri_(uri),
      doc_() {
      MCC_ASSERT(uri.HasScheme("file"));
      MCC_ASSERT(uri.HasExtension(".json"));
      LOG_IF(ERROR, !json::ParseJson(uri, doc_)) << "failed to parse ShaderDocument from: " << uri;
    }
    virtual ~ShaderDocument() = default;

    uri::Uri uri() const {
      return uri_;
    }

#define DEFINE_GET_SHADER_DOCUMENT_PROPERTY(Name)                 \
    inline bool Has##Name##Property() const {                     \
      return doc().HasMember(k##Name##Property);                  \
    }                                                             \
    inline const json::Value& Get##Name##Property() const {       \
      MCC_ASSERT(Has##Name##Property());                          \
      return doc()[k##Name##Property];                            \
    }
    FOR_EACH_SHADER_DOCUMENT_PROPERTY(DEFINE_GET_SHADER_DOCUMENT_PROPERTY)
#undef DEFINE_GET_SHADER_DOCUMENT_PROPERTY
  };
}

#endif //MCC_SHADER_DOCUMENT_H