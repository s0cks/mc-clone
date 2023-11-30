#ifndef MCC_JSON_H
#define MCC_JSON_H

#include <cstdio>
#include <glog/logging.h>
#include <rapidjson/writer.h>
#include <rapidjson/schema.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>

#include "mcc/common.h"

namespace mcc::json {
  using namespace rapidjson;

  bool ParseJson(FILE* file, Document& doc);
  
  static inline bool
  ParseJson(const std::string& filename, Document& doc) {
    DLOG(INFO) << "parsing json from " << filename;
    auto file = fopen(filename.c_str(), "rb");
    if(!file)
      return false;
    return ParseJson(file, doc);
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
}

#endif //MCC_JSON_H