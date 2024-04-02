#ifndef MCC_SHADER_COMPILER_H
#define MCC_SHADER_COMPILER_H

#include <fmt/format.h>

#include "mcc/rx.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/source.h"

namespace mcc::shader {
  class ShaderSourceCompiler {
    DEFINE_NON_COPYABLE_TYPE(ShaderSourceCompiler);
  public:
    ShaderSourceCompiler() = default;
    virtual ~ShaderSourceCompiler() = default;
    virtual rx::observable<ShaderId> Compile(const ShaderCodePtr source);
  };
}

#endif //MCC_SHADER_COMPILER_H