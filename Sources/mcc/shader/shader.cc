#include "mcc/flags.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/compiler.h"
#include "mcc/shader/builder.h"

namespace mcc::shader {
  static inline std::string
  GetShaderFilename(const std::string& rel) {
    return FLAGS_resources + "/shaders/" + rel;
  }

  Shader CompileShader(const std::string& name) {
    ProgramBuilder builder;
    {
      const auto source = Source::FromFile(GetShaderFilename(name + ".vsh"));
      const auto result = builder.AttachVertexShader(source);
      LOG_IF(FATAL, !result.success()) << "failed to compile vertex shader: " << result;
    }

    {
      const auto source = Source::FromFile(GetShaderFilename(name + ".fsh"));
      const auto result = builder.AttachFragmentShader(source);
      LOG_IF(FATAL, !result.success()) << "failed to compile fragment shader: " << result;
    }

    Shader shader;
    auto result = builder.Build(&shader);
    LOG_IF(ERROR, !result.success()) << "failed to compile shader: " << result;
    return shader;
  }
}