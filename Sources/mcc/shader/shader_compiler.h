#ifndef MCC_SHADER_COMPILER_H
#define MCC_SHADER_COMPILER_H

#include <fmt/format.h>

#include "mcc/rx.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/shader_code.h"

namespace mcc::shader {
  class ShaderCompiler {
    DEFINE_NON_COPYABLE_TYPE(ShaderCompiler);
  public:
    ShaderCompiler() = default;
    virtual ~ShaderCompiler() = default;
    virtual ShaderId Compile(ShaderCode* source);
  };
}

#endif //MCC_SHADER_COMPILER_H