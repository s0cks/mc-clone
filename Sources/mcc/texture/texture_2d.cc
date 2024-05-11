#include "mcc/texture/texture.h"

#include <sstream>
#include "mcc/json_spec.h"
#include "mcc/image/image.h"
#include "mcc/texture/texture_2d_loader_file.h"
#include "mcc/texture/texture_2d_loader_json.h"

namespace mcc::texture {
  std::string Texture2d::ToString() const {
    std::stringstream ss;
    ss << "Texture2d(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  Texture2d* Texture2d::New(const json::TextureValue* value) {
    MCC_ASSERT(value);
    MCC_ASSERT(value->IsObject());
    TextureBuilder builder(k2D);
    {
      const auto filter = value->GetFilter();
      if(filter)
        builder.SetFilter(*filter);
    }
    {
      const auto wrap = value->GetWrap();
      if(wrap)
        builder.SetWrap(*wrap);
    }
    //TODO: set border
    //TODO: set level
    //TODO: set mipmap
    //TODO: decode image data
    return new Texture2d(builder.Build());
  }

  Texture2d* Texture2d::New(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("tex") || uri.HasScheme("texture"));
    const auto uri_path = fmt::format("{0:s}/textures/{1:s}", FLAGS_resources, uri.path);
    const auto extension = uri.GetExtension();
    if(extension.empty()) {
      {
        const auto json_target = uri::Uri(fmt::format("file://{0:s}.json", uri_path));
        if(FileExists(json_target)) {
          NOT_IMPLEMENTED(FATAL); //TODO: implement
          return nullptr;
        }
      }

      {
        const auto valid_extensions = img::GetValidExtensions();
        for(const auto& ext : valid_extensions) {
          const auto test_target = uri::Uri(fmt::format("file://{0:s}.{1:s}", uri_path, ext));
          if(img::Filter(test_target) && FileExists(test_target))
            return Texture2dFileLoader::Load(test_target);
        }
      }
    }

    const auto target = uri::Uri(fmt::format("file://{0:s}", uri_path));
    if(EqualsIgnoreCase(extension, "json")) {
      json::Document doc;
      if(!json::ParseJson(target, doc)) {
        LOG(ERROR) << "failed to parse Texture2d json from: " << target;
        return nullptr;
      }
      NOT_IMPLEMENTED(FATAL); //TODO: implement
      return nullptr;
    } else if(img::Filter(target)) {
      return Texture2dFileLoader::Load(target);
    }

    LOG(ERROR) << "invalid texture uri: " << uri;
    return nullptr;
  }
}