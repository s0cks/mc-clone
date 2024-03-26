#include <memory>
#include <fmt/format.h>

#include "mcc/flags.h"
#include "mcc/file_resolver.h"
#include "mcc/material/material_json.h"

namespace mcc::material {
  class MaterialSchemaDocument;
  typedef std::shared_ptr<MaterialSchemaDocument> MaterialSchema;
  class MaterialSchemaDocument {
  protected:
    std::string filename_;
    json::SchemaDocument doc_;
  public:
    MaterialSchemaDocument(const std::string& filename, const json::Document& doc):
      filename_(filename),
      doc_(doc) {
    }
    virtual ~MaterialSchemaDocument() = default;

    std::string filename() const {
      return filename_;
    }
    
    virtual bool IsValid(const json::Document& doc) const {
      json::SchemaValidator validator(doc_);
      return doc.Accept(validator);
    }

    friend std::ostream& operator<<(std::ostream& stream, const MaterialSchemaDocument& rhs) {
      stream << "MaterialSchemaDocument(";
      stream << "filename=" << rhs.filename_;
      stream << ")";
      return stream;
    }
  };

  static MaterialSchema schema_;

  static inline MaterialSchema
  NewMaterialSchema(const std::string& filename) {
    if(!FileExists(filename)) {
      DLOG(ERROR) << "material schema document " << filename << " doesn't exist.";
      return nullptr;
    }
    json::Document doc;
    if(!json::ParseJson(filename, doc)) {
      DLOG(ERROR) << "failed to parse material schema document from: " << filename;
      return nullptr;
    }
    return std::make_shared<MaterialSchemaDocument>(filename, doc);
  }

  static inline std::optional<std::string>
  GetMaterialSchemaFilename() {
    PathResolver resolver(FLAGS_resources + "/materials", HasExtension(".schema.json"));
    return resolver.Resolve();
  }

  static inline MaterialSchema
  GetMaterialSchema() {
    if(schema_)
      return schema_;

    const auto filename = GetMaterialSchemaFilename();
    if(!filename) {
      DLOG(ERROR) << "cannot find MaterialSchema file in " << FLAGS_resources;
      return nullptr;
    }
    return schema_ = NewMaterialSchema((*filename));
  }

  bool MaterialDocument::IsValid() const {
    if(doc_.HasParseError()) {
      DLOG(ERROR) << "document " << filename_ << " has a parse error.";
      return false; //TODO: better error handling
    }
    MaterialSchema schema = GetMaterialSchema();
    if(!schema) {
      DLOG(ERROR) << "cannot get MaterialSchemaDocument.";
      return false;
    }
    return schema->IsValid(doc_);
  }
}