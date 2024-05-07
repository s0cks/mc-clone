#include "mcc/shader/shader_json.h"
#include "mcc/common.h"

namespace mcc::json {
  static inline void Test() {
    json::Document doc;
    json::ParseJson("", doc);
    const auto root = doc.GetObject();
    json::SpecDocument<json::ConstShaderObject> spec(root);
    const auto name = spec.GetName();
    const auto type = spec.GetType();
    const auto data = spec.GetData();
  }
}