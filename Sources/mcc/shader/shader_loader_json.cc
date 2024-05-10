#include "mcc/shader/shader_loader_json.h"
#include "mcc/shader/shader_uri.h"
#include "mcc/shader/shader_json.h"

namespace mcc::shader {
  std::string JsonShaderLoader::GetShaderSource() const {
    if(value().IsString()) {
      return std::string(value().GetString(), value().GetStringLength());
    } else if(value().IsObject()) {
      NOT_IMPLEMENTED(FATAL); //TODO: implement
      return {};
    }
    return {};
  }

  static inline std::optional<uri::Uri>
  TryParseShaderUri(const std::string& source) {
    if(StartsWith(source, "shader:") || StartsWith(source, "shader://")) {
      return { uri::Uri(source) };
    } else if(StartsWith(source, "file://") || StartsWith(source, "file:")) {
      return { uri::Uri(source) };
    }
    return std::nullopt;
  }

  Shader* JsonShaderLoader::LoadShaderFromUri(const uri::Uri& uri) const {
    MCC_ASSERT(IsValidShaderJsonUri(uri));
  }

  Shader* JsonShaderLoader::LoadShaderFromSource(const std::string& source) const {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }

  Shader* JsonShaderLoader::LoadShader() const {
    const auto source = GetShaderSource();
    if(source.empty()) {
      LOG(ERROR) << "shader source empty.";
      return nullptr;
    }
    const auto uri = TryParseShaderUri(source);
    return uri
         ? LoadShaderFromUri(*uri)
         : LoadShaderFromSource(source);
  }
}