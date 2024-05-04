#ifndef MCC_SHADER_SPEC_H
#error "Please #include <mcc/shader/shader_spec.h> instead."
#endif //MCC_SHADER_SPEC_H

#ifndef MCC_SHADER_SPEC_JSON_H
#define MCC_SHADER_SPEC_JSON_H

#include "mcc/shader/shader_spec.h"

namespace mcc::shader {
  class JsonShaderSpec : public ShaderSpec {

  public:
    explicit JsonShaderSpec(const uri::Uri& uri):
      ShaderSpec() {
    }
    ~JsonShaderSpec() override = default;
    std::string GetShaderName() const override;
    ShaderType GetShaderType() const override;
    ShaderCode* GetShaderCode() const override;
  };
}

#endif //MCC_SHADER_SPEC_JSON_H