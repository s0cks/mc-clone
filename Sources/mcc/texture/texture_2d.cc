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
    ss << "id=" << GetTextureId();
    ss << ")";
    return ss.str();
  }

  static inline std::string
  GetTextureName(const json::SpecDocument& doc) {
    MCC_ASSERT(doc.HasNameProperty());
    const auto& name = doc.GetNameProperty();
    MCC_ASSERT(name.IsString());
    MCC_ASSERT(name.GetStringLength() > 0);
    return std::string(name.GetString(), name.GetStringLength());
  }

  static inline std::string
  GetTextureType(const json::SpecDocument& doc) {
    MCC_ASSERT(doc.HasTypeProperty());
    const auto& type = doc.GetTypeProperty();
    MCC_ASSERT(type.IsString());
    MCC_ASSERT(type.GetStringLength() > 0);
    return std::string(type.GetString(), type.GetStringLength());
  }

  Texture2d* Texture2d::New(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("tex") || uri.HasScheme("texture"));
    const auto uri_path = fmt::format("{0:s}/textures/{1:s}", FLAGS_resources, uri.path);
    const auto extension = uri.GetPathExtension();
    if(!extension) {
      {
        const auto json_target = uri::Uri(fmt::format("file://{0:s}.json", uri_path));
        if(FileExists(json_target)) {

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
    if(EqualsIgnoreCase(*extension, "json")) {
      json::Document doc;
      if(!json::ParseJson(target, doc)) {
        LOG(ERROR) << "failed to parse Texture2d json from: " << target;
        return nullptr;
      }
      MCC_ASSERT(doc.IsObject());
      json::SpecDocument spec(doc.GetObject());
      MCC_ASSERT(spec.HasNameProperty());
      const auto name = GetTextureName(spec);
      MCC_ASSERT(spec.HasTypeProperty());
      const auto type = GetTextureType(spec);
      MCC_ASSERT(spec.HasSpecProperty());
      const auto& spec_prop = spec.GetSpecProperty();
      MCC_ASSERT(spec_prop.IsObject());
      const auto spec_prop_obj = spec_prop.GetObject();
      JsonTexture2dLoader loader(spec_prop_obj);
      return loader.LoadTexture();
    } else if(img::Filter(target)) {
      return Texture2dFileLoader::Load(target);
    }

    LOG(ERROR) << "invalid texture uri: " << uri;
    return nullptr;
  }
}