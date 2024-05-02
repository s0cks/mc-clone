#ifndef MCC_MATERIAL_SPEC_H
#error "Please #include <mcc/material/material_spec.h> instead."
#endif //MCC_MATERIAL_SPEC_H

#ifndef MCC_MATERIAL_SPEC_JSON_H
#define MCC_MATERIAL_SPEC_JSON_H

#include "mcc/material/material_spec.h"
#include "mcc/material/material_document.h"

#include "mcc/texture/texture_spec.h"

namespace mcc::material {
  class JsonMaterialSpec : public MaterialSpec {
  public:
    static constexpr const auto kAlbedoPropertyName = "albedo";
  protected:
    const json::ConstObject& doc_;
  public:
    explicit JsonMaterialSpec(const json::ConstObject& doc):
      doc_(doc) {
    }
    explicit JsonMaterialSpec(const json::Value& doc):
      JsonMaterialSpec(doc.GetObject()) {
    }
    ~JsonMaterialSpec() override = default;

    const json::Value& GetAlbedoProperty() const {
      return doc_[kAlbedoPropertyName];
    }
  };
}

#endif //MCC_MATERIAL_SPEC_JSON_H