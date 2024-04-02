#ifndef MCC_SHADER_COMPILER_H
#define MCC_SHADER_COMPILER_H

#include <fmt/format.h>

#include "mcc/rx.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/source.h"

namespace mcc::shader {
  class ShaderCompiler {
    DEFINE_NON_COPYABLE_TYPE(ShaderCompiler);
  public:
    ShaderCompiler() = default;
    virtual ~ShaderCompiler() = default;
    virtual ShaderId Compile(const ShaderCodePtr source);
    virtual rx::observable<ShaderId> Compile(rx::observable<ShaderCodePtr> sources);
  };
}

#endif //MCC_SHADER_COMPILER_H