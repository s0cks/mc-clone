#include "mcc/shader/shader_document.h"

namespace mcc::json {
  ShaderSpecDocument* ShaderSpec::New(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("file"));
    MCC_ASSERT(uri.HasExtension(".json"));
    return SpecDocument<ShaderSpec>::New(uri);
  }
}