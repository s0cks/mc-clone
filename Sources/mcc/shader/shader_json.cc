#include "mcc/shader/shader_json.h"
#include "mcc/thread_local.h"

namespace mcc::shader {
  class ShaderDocumentSchemaValidator {
  private:
    json::SchemaDocument schema_;
  public:
    ShaderDocumentSchemaValidator(const json::Document& doc):
      schema_(doc) {
    }
    virtual ~ShaderDocumentSchemaValidator() = default;
    
    virtual bool IsValid(const json::Document& doc) const {
      json::SchemaValidator validator(schema_);
      if(!doc.Accept(validator)) {
        //TODO:
        // StringBuffer sb;
        // validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
        // printf("Invalid schema: %s\n", sb.GetString());
        // printf("Invalid keyword: %s\n", validator.GetInvalidSchemaKeyword());
        // sb.Clear();
        // validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
        // printf("Invalid document: %s\n", sb.GetString());
        return false;
      }

      return true;
    }

    static inline ShaderDocumentSchemaValidator* 
    New(const std::string& schema_filename) {
      json::Document schema_doc;
      if(!json::ParseJson(schema_filename, schema_doc)) {
        DLOG(ERROR) << "no schema document found at: " << schema_filename;
        return nullptr;
      }
      return new ShaderDocumentSchemaValidator(schema_doc);
    }
  };

  static ThreadLocal<ShaderDocumentSchemaValidator> validator_;

  static inline ShaderDocumentSchemaValidator*
  GetValidator() {
    auto validator = validator_.Get();
    if(!validator)
      validator_.Set(validator = ShaderDocumentSchemaValidator::New(FLAGS_resources + "/shader.schema.json"));
    return validator;
  } 

  bool IsValidShaderDocument(const json::Document& doc) {
    const auto validator = GetValidator();
    return validator->IsValid(doc);
  }
}