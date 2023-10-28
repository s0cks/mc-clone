#include "mcc/flags.h"
#include "mcc/shader/shader.h"

namespace mcc {
  static inline std::string
  GetShaderFilename(const std::string& rel) {
    return FLAGS_resources + "/shaders/" + rel;
  }

  Shader CompileShader(const std::string& name) {
    ProgramCompiler compiler;
    const auto vertResult = compiler.AttachVertexShaderFromFile(GetShaderFilename(name + ".vsh"));
    LOG_IF(FATAL, !vertResult.success()) << "failed to compile vertex shader: " << vertResult;
    const auto fragResult = compiler.AttachFragmentShaderFromFile(GetShaderFilename(name + ".fsh"));
    LOG_IF(FATAL, !fragResult.success()) << "failed to compile fragment shader: " << fragResult;
    
    Shader shader;
    GlCompiler::CompilationResult result;
    result = compiler.Compile(&shader);
    LOG_IF(ERROR, !result.success()) << "failed to compile shader: " << result;
    return shader;
  }
}