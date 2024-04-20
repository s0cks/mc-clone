#include "mcc/shader/shader_code.h"

#include <fmt/format.h>

#include "mcc/flags.h"

namespace mcc::shader {
  ShaderCode* ShaderCode::FromFile(const uri::Uri& uri) {
    if(uri.HasScheme("shader")) {
      MCC_ASSERT(uri.HasExtension());
      const auto abs_path = fmt::format("file://{0:s}/shaders/{1:s}", FLAGS_resources, uri.path);
      return FromFile(uri::Uri(abs_path));
    }

    MCC_ASSERT(uri.HasScheme("file"));
    const auto type = DetectShaderTypeFromExtension(uri.GetPathExtension());
    if(!type) {
      DLOG(ERROR) << "cannot determine ShaderType from: " << uri;
      return nullptr;
    }
    return FromFile(*type, uri);
  }
}