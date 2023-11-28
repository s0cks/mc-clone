#include <unordered_map>

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

  ShaderRef GetShader(const uri::Uri& uri) {
    MCC_ASSERT(uri.scheme == "shader");
    //TODO: cache
    const auto location = FLAGS_resources + "/shaders";
    ProgramBuilder builder;
    {
      const auto source = Source::FromFile(GetVertexShaderFilename(location + uri.path));
      const auto result = builder.AttachVertexShader(source);
      LOG_IF(FATAL, !result.success()) << "failed to compile vertex shader: " << result;
    }

    {
      const auto source = Source::FromFile(GetFragmentShaderFilename(location + uri.path));
      const auto result = builder.AttachFragmentShader(source);
      LOG_IF(FATAL, !result.success()) << "failed to compile fragment shader: " << result;
    }

    auto shader = new Shader();
    auto result = builder.Build(shader);
    LOG_IF(ERROR, !result.success()) << "failed to compile shader: " << result;
    return ShaderRef(shader);
  }
}