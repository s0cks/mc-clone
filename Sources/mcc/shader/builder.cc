#include "mcc/shader/builder.h"
#include "mcc/shader/compiler.h"

namespace mcc::shader {
  CompilerResult ProgramBuilder::Attach(const Shader& shader) {
    glAttachShader(id_, shader.GetShaderId());
    CHECK_GL(FATAL);
    return Success();
  }

  CompilerResult ProgramBuilder::AttachVertexShader(const Source& source) {
    return Attach<VertexShaderCompiler>(source);
  }

  CompilerResult ProgramBuilder::AttachFragmentShader(const Source& source) {
    return Attach<FragmentShaderCompiler>(source);
  }
}