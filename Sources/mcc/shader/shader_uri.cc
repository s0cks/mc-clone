#include "mcc/shader/shader_uri.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/shader_flags.h"

namespace mcc::shader {
  const std::set<std::string>&
  GetValidUriSchemes() {
    static std::set<std::string> kValidSchemes;
    if(kValidSchemes.empty()) {
      kValidSchemes.insert("shader");
      kValidSchemes.insert("file");
    }
    return kValidSchemes;
  }

  bool IsValidShaderUri(const uri::Uri& uri) {
    return uri.HasScheme(GetValidUriSchemes());
  }

  bool IsValidShaderJsonUri(const uri::Uri& uri) {
    return IsValidShaderUri(uri) && uri.HasExtension("json");
  }

  void NormalizeShaderUriPath(uri::Uri& uri) {
    const auto root = GetShaderResourcesDirectory();
    auto& path = uri.path;
    if(StartsWith(path, root))
      return;
    path = fmt::format("{0:s}/{1:s}", root, path);
  }
}