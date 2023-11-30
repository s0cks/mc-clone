#include <unordered_map>

#include "mcc/flags.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/compiler.h"
#include "mcc/shader/builder.h"

#include "mcc/shader/shader_resolver.h"

namespace mcc {
  ShaderRef GetShader(const uri::Uri& uri) {
    MCC_ASSERT(uri.scheme == "shader");
    std::vector<std::string> results;
    ShaderResolver resolver(FLAGS_resources + "/shaders", results);
    if(!resolver.Resolve(uri)) {
      DLOG(ERROR) << "failed to resolve shaders for: " << uri;
      return ShaderRef();
    }
    DLOG(INFO) << "resolved:";
    for(const auto& result : results)
      DLOG(INFO) << " - " << result;

    return ShaderRef();
  }
}