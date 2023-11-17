#ifndef MCC_TEXTURE_JSON_H
#define MCC_TEXTURE_JSON_H

#include <optional>
#include "mcc/json.h"
#include "mcc/texture/texture.h"

namespace mcc::texture {
  void InitializeSchemaDocument();
  json::SchemaDocument* GetSchemaDocument();

  static inline bool
  HasSchemaDocument() {
    return GetSchemaDocument() != nullptr;
  }

  bool IsValidTextureDocument(json::Document& doc);

  static inline bool
  IsValidTextureDocument(const std::string& filename) {
    json::Document doc;
    if(!json::ParseJson(filename, doc)) {
      DLOG(ERROR) << "failed to parse texture document at: " << filename;
      return false;
    }
    return IsValidTextureDocument(doc);
  }

  class TextureDocument {
    static constexpr const char* kFilenamePropertyName = "filename";
    static constexpr const char* kFilterPropertyName = "filter";
    static constexpr const char* kFilterMinPropertyName = "min";
    static constexpr const char* kFilterMagPropertyName = "mag";
    static constexpr const char* kAlignmentPropertyName = "alignment";
  private:
    json::Document& doc_;
  public:
    TextureDocument(json::Document& doc):
      doc_(doc) {  
    }
    ~TextureDocument() = default;

    std::string GetFilename() const {
      const auto& value = doc_[kFilenamePropertyName];
      return std::string(value.GetString(), value.GetStringLength());
    }
  };
}

#endif //MCC_TEXTURE_JSON_H