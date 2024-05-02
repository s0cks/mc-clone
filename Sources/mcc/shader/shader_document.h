#ifndef MCC_SHADER_DOCUMENT_H
#define MCC_SHADER_DOCUMENT_H

#include "mcc/spec.h"
#include "mcc/shader/shader_type.h"

namespace mcc::json {
  class ShaderSpec;
  typedef SpecDocument<ShaderSpec> ShaderSpecDocument;
  class ShaderSpec {
    static constexpr const auto kTypeProperty = "type";
    static constexpr const auto kSourceProperty = "source";
  protected:
    const json::ConstObject& doc_;
  public:
    explicit ShaderSpec(const json::ConstObject& doc):
      doc_(doc) {
    }
    explicit ShaderSpec(const json::Value& doc):
      ShaderSpec(doc.GetObject()) {
    }
    virtual ~ShaderSpec() = default;

    const json::Value& GetTypeProperty() const {
      return doc_[kTypeProperty];
    }

    const json::Value& GetSourceProperty() const {
      return doc_[kSourceProperty];
    }
  public:
    static ShaderSpecDocument* New(const uri::Uri& uri);
  };
}

#endif //MCC_SHADER_DOCUMENT_H