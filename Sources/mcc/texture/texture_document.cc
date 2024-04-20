#include "mcc/texture/texture_document.h"

#include <optional>

namespace mcc::texture {
  static constexpr const auto kTypePropertyName = "type";
  static constexpr const auto kSpecPropertyName = "spec";

  static inline std::optional<std::string>
  GetDocumentType(const json::Document& doc) {
    if(!doc.HasMember(kTypePropertyName)) {
      DLOG(ERROR) << "no type property in document.";
      return std::nullopt;
    }
    const auto& type = doc[kTypePropertyName];
    if(!type.IsString()) {
      DLOG(ERROR) << "type field is not a string.";
      return std::nullopt;
    }
    return { std::string(type.GetString(), type.GetStringLength()) };
  }

  TextureDocument* TextureDocument::New(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("file"));
    MCC_ASSERT(uri.HasExtension("json"));
    
    json::Document doc;
    if(!json::ParseJson(uri.path,  doc)) {
      LOG(ERROR) << "failed to parse TextureDocument from: " << uri;
      return nullptr;
    }

    //TODO: validate schema
    const auto type = GetDocumentType(doc);
    if(!type) {
      LOG(ERROR) << "couldn't get document type from: " << uri;
      return nullptr;
    }
    if(!doc.HasMember(kSpecPropertyName)) {
      LOG(ERROR) << "no spec property in texture document: " << uri;
      return nullptr;
    }
    const auto& spec = doc[kSpecPropertyName];

    if(EqualsIgnoreCase(*type, "texture2d")) {
      return Texture2DDocument::New(spec.GetObject());
    } else if(EqualsIgnoreCase(*type, "texture3d")) {
      NOT_IMPLEMENTED(FATAL);//TODO: implement
      return nullptr;
    } else if(EqualsIgnoreCase(*type, "cubemap")) {
      NOT_IMPLEMENTED(FATAL);//TODO: implement
      return nullptr;
    }

    LOG(ERROR) << "unknown texture document type: " << (*type);
    return nullptr;
  }
}