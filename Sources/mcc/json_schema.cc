#include "mcc/json_schema.h"

#include "mcc/json_flags.h"
#include "mcc/thread_local.h"

namespace mcc::json {
  Schema::ValidationResult Schema::Validate(const json::Document& doc) const {
    if(doc.HasParseError())
      return ValidationResult(false);
    SchemaValidator validator(doc_);
    const auto valid = doc.Accept(validator);
    return ValidationResult(valid, &validator);
  }

  rx::observable<Schema::ValidationResult> Schema::ValidateAsync(const json::Document& doc) const {
    if(doc.HasParseError())
      return rx::observable<>::error<ValidationResult>(std::runtime_error("Document is not valid JSON"));
    return rx::observable<>::create<ValidationResult>([](rx::subscriber<ValidationResult> s) {

    });
  }

  static ThreadLocal<Schema> schema_;

  Schema* Schema::New(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("file"));
    MCC_ASSERT(uri.HasExtension("json"));
    json::Document doc;
    if(!json::ParseJson(uri, doc)) {
      LOG(ERROR) << "failed to parse Schema JSON from: " << uri;
      return nullptr;
    }
    return new Schema(doc);
  }

  Schema* GetSchema() {
    if(schema_)
      return schema_.Get();
    const auto schema = Schema::New();
    schema_.Set(schema);
    return schema;
  }

  void SetSchema(Schema* schema) {
    MCC_ASSERT(schema);
    schema_.Set(schema);
  }
}