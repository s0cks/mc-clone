#include <unordered_map>

#include "mcc/flags.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/shader_linker.h"
#include "mcc/shader/shader_compiler.h"
#include "mcc/shader/shader_source_resolver.h"

namespace mcc {
  static inline bool
  HasShaderExtension(const uri::Uri& uri) {
    const auto extension = uri.GetPathExtension();
    if(!extension)
      return false;
#define CHECK_EXTENSION(Name, Ext, GlValue) \
    else if(EqualsIgnoreCase((*extension), (#Ext))) return true;
    FOR_EACH_SHADER_TYPE(CHECK_EXTENSION)
#undef CHECK_EXTENSION
    return false;
  }

  ShaderRef GetShader(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("shader"));
    shader::ShaderSourceResolver resolver(uri);
    auto sources = resolver.Resolve().map([](const shader::ShaderSource& next) {
      return next.GetSource();
    });
    shader::ShaderCompiler compiler;
    shader::ShaderLinker linker;
    const auto shader = linker.Link(compiler.Compile(sources))
      .map([](const shader::ShaderId& next) {
        return new Shader(next);
      })
      .as_blocking()
      .first();
    return ShaderRef(shader);
  }
}