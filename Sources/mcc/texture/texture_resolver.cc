#include "mcc/texture/texture_resolver.h"

namespace mcc::texture {
  static const std::vector<std::string> kTextureExtensions = {
    ".json",
    ".png",
    ".jpg",
    ".jpeg",
  };

  std::optional<std::string> TextureResolver::Resolve(const uri::Uri& uri) const {
    MCC_ASSERT(uri.scheme == "texture");
    auto target = root_ + uri.path;
    if(FileExists(target)) {
      if(IsDirectory(target)) {
        if(FileExists(target + "/texture.json")) {
          return std::optional<std::string>{ target + "/texture.json" };
        }

        return std::nullopt;
      }
      return std::optional<std::string>{ target };
    }

    if(HasExtension(uri))
      return std::nullopt;
    
    for(const auto& ext : kTextureExtensions) {
      if(FileExists(target + ext))
        return std::optional<std::string>{ target + ext };
    }
    return std::nullopt;
  }
}