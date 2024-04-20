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
    virtual ShaderId CompileShaderCode(ShaderCode* source);
  public:
    static inline ShaderId
    Compile(ShaderCode* code) {
      MCC_ASSERT(code);
      MCC_ASSERT(!code->IsEmpty());
      ShaderCompiler compiler;
      return compiler.CompileShaderCode(code);
    }

    static ShaderId Compile(const uri::Uri& uri);

    static inline ShaderId
    Compile(const uri::basic_uri& uri) {
      auto target = uri;
      if(!(StartsWith(target, "shader:") || StartsWith(target, "file:"))) {
        if(StartsWith(target, "/")) {
          target = fmt::format("file://{0:s}", target);
        } else {
          target = fmt::format("shader:{0:s}", target);
        }
      }
      return Compile(uri::Uri(target));
    }
  };
}

#endif //MCC_SHADER_COMPILER_H