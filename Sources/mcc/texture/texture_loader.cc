#include "mcc/texture/texture_loader.h"

#include <regex>
#include <fmt/format.h>

#include "mcc/common.h"
#include "mcc/texture/texture_json.h"
#include "mcc/texture/texture_builder.h"

namespace mcc::texture {
  static inline bool
  IsAbsolutePath(const uri::Uri& uri) {
    auto path = uri.path;
    if(FileExists(path))
      return true;
    path = fmt::format("{0:s}/textures/{1:s}", FLAGS_resources, path);
    return FileExists(path);
  }

  static const auto kValidImageExtensions = {
    ".png",
    ".jpeg",
    ".jpg"
  };

  std::optional<Texture*> TextureLoader::Load(const uri::Uri& uri) {
    // Check for abs path
    auto path = uri.path;
    if(FileExists(path)) {

    }
    path = fmt::format("{0:s}/textures/{1:s}", FLAGS_resources, path);
    if(FileExists(path)) {

    }

    if(uri.HasPathExtension()) {
      DLOG(ERROR) << "cannot get Texture from: " << path;
      return std::nullopt;
    }

    TextureBuilder builder;
    // Check for path + .json
    if(FileExists(path + ".json")) {
      json::Document doc;
      if(!json::ParseJson(path + ".json", doc) || doc.HasParseError()) {
        DLOG(ERROR) << "failed to parse Texture json document: " << (path + ".json");
        return std::nullopt;
      }

      JsonTexture texture(doc);
      if(!texture.IsValid()) {
        DLOG(ERROR) << "Texture json document " << (path + ".json") << " is invalid.";
        return std::nullopt;
      }

      auto filename = texture.GetFilename();
      if(!filename) {
        for(const auto& extension : kValidImageExtensions) {
          const auto path_and_extension = path + extension;
          if(FileExists(path_and_extension)) {
            filename = path_and_extension;
            break;
          }
        }
      }

      if(!FileExists(*filename)) {
        DLOG(ERROR) << "cannot find texture image: " << *filename;
        return std::nullopt;
      }

      const auto target = texture.GetTarget();
      if(target)
        builder.SetTarget((*target));
      const auto filter = texture.GetFilter();
      if(filter)
        builder.SetFilter((*filter));
      const auto wrap = texture.GetWrap();
      if(wrap)
        builder.SetWrap((*wrap));
    }

    // Check for path + .png

    // Check for path + .jpeg|.jpg
    return std::nullopt;
  }
}