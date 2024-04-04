#include "mcc/shader/shader_spec_json.h"

namespace mcc::shader {
  JsonShaderSpec::JsonShaderSpec(const uri::Uri& uri, json::DocumentPtr doc):
    ShaderSpec(),
    json::JsonSpec(uri, doc) {
  }
}