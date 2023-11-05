#include "mcc/flags.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/compiler.h"
#include "mcc/shader/builder.h"

namespace mcc::shader {
  DEFINE_string(shader_cache_dir, kDefaultShaderCacheDir, "The directory for all stored shaders.");
  DEFINE_uint32(shader_cache_size, kDefaultShaderCacheSize, "The size of the shader cache.");
  DEFINE_uint32(shader_cache_buckets, kDefaultShaderCacheBuckets, "The number of buckets for the shader cache to use.");

  static LruCache<std::string, Shader> cache_(Shader::GetCacheCapacity());

  uint64_t Shader::GetCacheSize() {
    return cache_.GetSize();
  }

  Shader Shader::Get(const std::string& name, std::function<Shader(const std::string&)> load) {
    return cache_.Get(name, load);
  }

  static inline std::string
  GetShaderFilename(const std::string& name, const std::string& extension) {
    const auto root = Shader::GetCacheDirectory();
    auto filename = name;
    if(!StartsWith(filename, root))
      filename = root + "/" + name;
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

  const std::function<Shader(const std::string&)> Shader::kDefaultShaderCacheLoader = [](const std::string& name) {
    ProgramBuilder builder;
    {
      const auto source = Source::FromFile(GetVertexShaderFilename(name));
      const auto result = builder.AttachVertexShader(source);
      LOG_IF(FATAL, !result.success()) << "failed to compile vertex shader: " << result;
    }

    {
      const auto source = Source::FromFile(GetFragmentShaderFilename(name));
      const auto result = builder.AttachFragmentShader(source);
      LOG_IF(FATAL, !result.success()) << "failed to compile fragment shader: " << result;
    }

    Shader shader;
    auto result = builder.Build(&shader);
    LOG_IF(ERROR, !result.success()) << "failed to compile shader: " << result;
    return shader;
  };
}