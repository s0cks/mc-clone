#include <unordered_map>

#include "mcc/flags.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/compiler.h"
#include "mcc/shader/builder.h"

#include "mcc/shader/shader_resolver.h"

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
    if(HasShaderExtension(uri)) {
      const auto path = fmt::format("{0:s}/shaders/{1:s}", FLAGS_resources, uri.path);
      DLOG(INFO) << "loading shader from: " << path;
    }

    return ShaderRef();
  }
}