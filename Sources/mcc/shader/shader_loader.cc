#include "mcc/shader/shader_loader.h"

#include "mcc/flags.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/compiler.h"
#include "mcc/shader/builder.h"

namespace mcc {
  static constexpr const auto kVertexShaderExtensions = {
    ".vs",
    ".vsh",
  };
  static constexpr const auto kFragmentShaderExtensions = {
    ".fs",
    ".fsh",
  };

  class DefaultShaderLoader : public ShaderLoader {
  protected:
    explicit DefaultShaderLoader(const uri::Uri& target):
      ShaderLoader(target) {
    }
  public:
    ~DefaultShaderLoader() override = default;
    ShaderRef Load() override = 0;
  };

  ShaderRef DefaultShaderLoader::Load() {
    MCC_ASSERT(target().scheme == "shader");
    // //TODO: cache
    // const auto location = FLAGS_resources + "/shaders";
    // ProgramBuilder builder;
    // {
    //   const auto source = Source::FromFile(GetVertexShaderFilename(location + uri.path));
    //   const auto result = builder.AttachVertexShader(source);
    //   LOG_IF(FATAL, !result.success()) << "failed to compile vertex shader: " << result;
    // }

    // {
    //   const auto source = Source::FromFile(GetFragmentShaderFilename(location + uri.path));
    //   const auto result = builder.AttachFragmentShader(source);
    //   LOG_IF(FATAL, !result.success()) << "failed to compile fragment shader: " << result;
    // }

    // auto shader = new Shader();
    // auto result = builder.Build(shader);
    // LOG_IF(ERROR, !result.success()) << "failed to compile shader: " << result;
    // return ShaderRef(shader);
  }
}