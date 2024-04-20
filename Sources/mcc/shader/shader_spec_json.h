#ifndef MCC_SHADER_SPEC_H
#error "Please #include <mcc/shader/shader_spec.h> instead."
#endif //MCC_SHADER_SPEC_H

#ifndef MCC_SHADER_SPEC_JSON_H
#define MCC_SHADER_SPEC_JSON_H

#include "mcc/shader/shader_spec.h"
#include "mcc/shader/shader_document.h"

namespace mcc::shader {
  class JsonShaderSpec : public ShaderSpec {
  protected:
    ShaderDocument doc_;
    std::string name_;
    ShaderType type_;

    inline ShaderDocument& doc() {
      return doc_;
    }

    inline const ShaderDocument& doc() const {
      return doc_;
    }

    std::string GetShaderNameProperty() const;
    ShaderType GetShaderTypeProperty() const;
  public:
    explicit JsonShaderSpec(const uri::Uri& uri):
      ShaderSpec(),
      doc_(uri),
      name_(GetShaderNameProperty()),
      type_(GetShaderTypeProperty()) {
    }
    ~JsonShaderSpec() override = default;

    std::string GetShaderName() const override {
      return name_;
    }
    
    ShaderType GetShaderType() const override {
      return type_;
    }

    ShaderCode* GetShaderCode() const override;
  };
}

#endif //MCC_SHADER_SPEC_JSON_H