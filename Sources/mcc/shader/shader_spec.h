#ifndef MCC_SHADER_SPEC_H
#define MCC_SHADER_SPEC_H

#include <string>
#include "mcc/shader/shader_type.h"
#include "mcc/shader/shader_code.h"

namespace mcc::shader {
  class ShaderSpec {
  protected:
    ShaderSpec() = default;
  public:
    virtual ~ShaderSpec() = default;
    virtual std::string GetShaderName() const = 0;
    virtual ShaderType GetShaderType() const = 0;
    virtual ShaderCode* GetShaderCode() const = 0;
  };
}

#include "mcc/shader/shader_spec_json.h"

#endif //MCC_SHADER_SPEC_H