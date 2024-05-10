#include "mcc/program/program_loader_json.h"
#include "mcc/program/program_json.h"
#include "mcc/json_spec.h"
#include "mcc/json_schema.h"

#include "mcc/shader/shader_json.h"

namespace mcc::program {
  Program* JsonProgramLoader::LoadProgram() const {
    DLOG(INFO) << "loading Program from " << GetUri() << "....";
    json::Document doc;
    if(!json::ParseJson(uri_, doc)) {
      LOG(ERROR) << "failed to parse json from: " << uri_;
      return nullptr;
    }

    if(json::IsSchemaValidationEnabled()) {
      const auto schema = json::GetSchema();
      MCC_ASSERT(schema);
      const auto result = schema->Validate(doc);
      if(!result) {
        LOG(ERROR) << GetUri() << " schema is invalid: " << result;
        return nullptr;
      }
    }

    DLOG(INFO) << "spec: ";
    const auto root = doc.GetObject();
    json::SpecDocument<json::ConstProgramObject> spec(root);
    const auto name = spec.GetName();
    DLOG(INFO) << " - name: " << name;
    const auto type = spec.GetType();
    DLOG(INFO) << " - type: " << type;
    const auto data = spec.GetData();
    DLOG(INFO) << " - data: " << data;
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }
}