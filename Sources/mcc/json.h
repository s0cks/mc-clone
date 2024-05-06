#ifndef MCC_JSON_H
#define MCC_JSON_H

#include <cstdio>
#include <string>
#include <memory>
#include <optional>
#include <glog/logging.h>
#include <rapidjson/writer.h>
#include <rapidjson/schema.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>

#include "mcc/rx.h"
#include "mcc/uri.h"
#include "mcc/common.h"

namespace mcc::json {
  using namespace rapidjson;

  typedef Document::Object Object;
  typedef Document::ConstObject ConstObject;

  typedef std::shared_ptr<Document> DocumentPtr;

  bool ParseJson(FILE* file, Document& doc);
  bool ParseJson(const uri::Uri& uri, Document& doc);

  static inline bool
  ParseJson(const uri::basic_uri& uri, Document& doc) {
    return ParseJson(uri::Uri(uri), doc);
  }

  bool ParseRawJson(const std::string& json, Document& doc);

  static constexpr const auto kDefaultJsonFileBufferSize = 4096;
  rx::observable<DocumentPtr> ParseDocument(FILE* file, const int buffer_size = kDefaultJsonFileBufferSize);

  static inline rx::observable<DocumentPtr>
  ParseDocument(const uri::Uri& uri, const int buffer_size = kDefaultJsonFileBufferSize) {
    auto file = fopen(uri.path.c_str(), "rb");
    if(!file)
      return rx::observable<>::error<DocumentPtr>(std::runtime_error(""));
    return ParseDocument(file, buffer_size);
  }

  static inline std::string
  GetParseError(const Document& doc) {
    MCC_ASSERT(doc.HasParseError());
    std::stringstream ss;
    switch(doc.GetParseError()) {
      case kParseErrorDocumentEmpty:
        ss << "Document is empty.";
        break;
      case kParseErrorDocumentRootNotSingular:
        ss << "Document has multiple root values.";
        break;
      case kParseErrorValueInvalid:
        ss << "Valid is invalid.";
        break;
      default:
        ss << "Unknown: " << static_cast<uint64_t>(doc.GetParseError());
        break;
    }
    return ss.str(); //TODO: optimize and improve this
  }

  static inline bool
  Validate(SchemaDocument& schema, Document& doc) {
    SchemaValidator validator(schema);
    if (!doc.Accept(validator)) {
      // Input JSON is invalid according to the schema
      // Output diagnostic information
      StringBuffer sb;
      validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
      printf("Invalid schema: %s\n", sb.GetString());
      printf("Invalid keyword: %s\n", validator.GetInvalidSchemaKeyword());
      sb.Clear();
      validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
      printf("Invalid document: %s\n", sb.GetString());
      return false;
    }
    return true;
  }

  static inline bool
  Validate(SchemaDocument& schema, const std::string& filename) {
    Document doc;
    if(!ParseJson(filename, doc))
      return false;
    return Validate(schema, doc);
  }

  static inline bool
  Validate(const std::string& schemaf, const std::string& filename) {
    Document schemad;
    if(!ParseJson(schemaf, schemad))
      return false;
    SchemaDocument schema(schemad);
    return Validate(schema, filename);
  }

  class Spec;
  class SpecSchema {
    friend class Spec;
    friend class SpecSchemaTest;
  protected:
    json::SchemaDocument doc_;
    
    explicit SpecSchema(const json::Document& doc):
      doc_(doc) {
    }
  public:
    virtual ~SpecSchema() = default;
    virtual Spec* GetSpec() const = 0;
    
    virtual bool IsValid(const json::Document& doc) const {
      json::SchemaValidator validator(doc_);
      if(doc.Accept(validator))
        return true;
      //TODO: print errors
      // StringBuffer sb;
      // validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
      // printf("Invalid schema: %s\n", sb.GetString());
      // printf("Invalid keyword: %s\n", validator.GetInvalidSchemaKeyword());
      // sb.Clear();
      // validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
      // printf("Invalid document: %s\n", sb.GetString());
      // return false;
      return false;
    }
  };

  class JsonSpec {
  protected:
    uri::Uri uri_;
    DocumentPtr doc_;

    JsonSpec(const uri::Uri& uri, DocumentPtr doc):
      uri_(uri),
      doc_(doc) {
      MCC_ASSERT(doc.get());
      MCC_ASSERT(uri.HasScheme("file"));
      MCC_ASSERT(uri.HasExtension(".json"));
    }

    inline DocumentPtr doc() const {
      return doc_;
    }

    inline std::optional<std::string>
    GetStringProperty(const char* name) const {
      if(!doc_->HasMember(name))
        return std::nullopt;
      const auto& property = (*doc_)[name];
      if(!property.IsString())
        return std::nullopt;
      const std::string value(property.GetString(), property.GetStringLength());
      return { value };
    }

    inline std::optional<uri::Uri>
    GetUriProperty(const char* name) const {
      const auto property = GetStringProperty(name);
      return property ? std::optional<uri::Uri>(uri::Uri(*property)) : std::nullopt;
    }
  public:
    virtual ~JsonSpec() = default;
  };
}

#endif //MCC_JSON_H