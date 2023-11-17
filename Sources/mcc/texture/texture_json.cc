#include "mcc/texture/texture_json.h"

#include <glog/logging.h>
#include "mcc/flags.h"
#include "mcc/thread_local.h"

namespace mcc::texture {
  static ThreadLocal<json::SchemaDocument> schema;

  json::SchemaDocument* GetSchemaDocument() {
    return schema.Get();
  }

  void InitializeSchemaDocument() {
    const auto textures_schema = FLAGS_resources + "/textures/texture.schema.json";
    json::Document doc;
    if(!json::ParseJson(textures_schema, doc)) {
      LOG(FATAL) << "failed to parse JSON Schema at: " << textures_schema;
      return;
    }
    schema.Set(new json::SchemaDocument(doc));
  }

  bool IsValidTextureDocument(json::Document& doc) {
    json::SchemaValidator validator(*GetSchemaDocument());
    return doc.Accept(validator);
  }
}