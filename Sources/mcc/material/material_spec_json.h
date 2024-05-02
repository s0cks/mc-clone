#ifndef MCC_MATERIAL_SPEC_H
#error "Please #include <mcc/material/material_spec.h> instead."
#endif //MCC_MATERIAL_SPEC_H

#ifndef MCC_MATERIAL_SPEC_JSON_H
#define MCC_MATERIAL_SPEC_JSON_H

#include "mcc/material/material_spec.h"
#include "mcc/material/material_document.h"

namespace mcc::material {
  class JsonMaterialSpec : public MaterialSpec {
  protected:
    MaterialDocument doc_;
    std::string name_;
    MaterialComponentSet components_;

    static inline std::string
    GetNameProperty(const MaterialDocument& doc) {
      const auto& name = doc.GetNameProperty();
      MCC_ASSERT(name.IsString());
      return std::string(name.GetString(), name.GetStringLength());
    }
  public:
    explicit JsonMaterialSpec(const uri::Uri& uri):
      MaterialSpec(),
      doc_(uri),
      name_(GetNameProperty(doc_)),
      components_() {
    }
    ~JsonMaterialSpec() override = default;
    
    std::string GetName() const override {
      return name_;
    }

    const MaterialComponentSet& GetComponents() const override {
      return components_;
    }
  };
}

#endif //MCC_MATERIAL_SPEC_JSON_H