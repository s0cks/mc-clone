#include "mcc/flags.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/compiler.h"
#include "mcc/shader/builder.h"

namespace mcc {
  using namespace shader;

  static inline std::string
  GetShaderFilename(const std::string& name, const std::string& extension) {
    auto filename = name;
    if(!StartsWith(filename, FLAGS_resources + "/shaders/"))
      filename = FLAGS_resources + "/shaders/" + name;
    if(!EndsWith(filename, extension))
      filename = filename + extension;
    return filename;
  }

  static inline std::string
  GetVertexShaderFilename(const std::string& name) {
    return GetShaderFilename(name, kVertexShaderExtension);
  }

  static inline std::string
  GetFragmentShaderFilename(const std::string& name) {
    return GetShaderFilename(name, kFragmentShaderExtension);
  }

  ShaderRef GetShader(const resource::Token& token) {
    DLOG(INFO) << "getting shader from: " << token;
    ProgramBuilder builder;
    {
      const auto source = Source::FromFile(GetVertexShaderFilename(token.location));
      const auto result = builder.AttachVertexShader(source);
      LOG_IF(FATAL, !result.success()) << "failed to compile vertex shader: " << result;
    }

    {
      const auto source = Source::FromFile(GetFragmentShaderFilename(token.location));
      const auto result = builder.AttachFragmentShader(source);
      LOG_IF(FATAL, !result.success()) << "failed to compile fragment shader: " << result;
    }

    Shader shader;
    auto result = builder.Build(&shader);
    LOG_IF(ERROR, !result.success()) << "failed to compile shader: " << result;
    const auto ptr = resource::Pointer(token.tag,(uword) (new Shader(shader)));
    return ShaderRef(ptr);
  }
}