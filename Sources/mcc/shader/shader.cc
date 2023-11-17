#include <unordered_map>

#include "mcc/flags.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/compiler.h"
#include "mcc/shader/builder.h"

namespace mcc {
  using namespace shader;

  static std::unordered_map<std::string, Shader*> all_shaders_;

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

  static inline ShaderRef
  CreateReferenceTo(const Shader* value, const res::Tag& tag = res::Tag::Shader()) {
    const auto ptr = resource::Pointer(tag, (uword) value);
    return ShaderRef(ptr);
  }

  ShaderRef GetShader(const std::string& name) {
    const auto& pos = all_shaders_.find(name);
    if(pos != all_shaders_.end())
      return CreateReferenceTo(pos->second, res::Tag::Shader(name));
    return GetShader(res::Registry::Get(res::Tag::Shader(name)));
  }

  ShaderRef GetShader(const res::Token& token) {
    const auto pos = all_shaders_.find(token.tag.value());
    if(pos != all_shaders_.end())
      return CreateReferenceTo(pos->second, token.tag);

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

    auto shader = new Shader();
    auto result = builder.Build(shader);
    LOG_IF(ERROR, !result.success()) << "failed to compile shader: " << result;
    all_shaders_.insert({ token.tag.value(), shader });
    return CreateReferenceTo(shader, token.tag);
  }
}