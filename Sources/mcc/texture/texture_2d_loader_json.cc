#include "mcc/texture/texture_2d_loader_json.h"
#include "mcc/texture/texture_builder.h"
#include "mcc/image/image.h"

namespace mcc::texture {
  Texture2d* JsonTexture2dLoader::LoadTexture() const {
    TextureBuilder builder(k2D);
    const auto filter = doc().GetFilterProperty();
    if(filter) {
      const auto filter_value = filter.value();
      if(filter_value->IsObject()) {
        builder.SetFilter(TextureFilter(filter_value->GetObject()));
      } else if(filter_value->IsString()) {
        std::string filter_string(filter_value->GetString(), filter_value->GetStringLength());
        builder.SetFilter(TextureFilter(filter_string));
      } else {
        LOG(FATAL) << "invalid filter poperty type.";
        return nullptr;
      }
    }

    const auto wrap = doc().GetWrapProperty();
    if(wrap) {
      if((*wrap)->IsObject()) {
        builder.SetWrap(TextureWrap((*wrap)->GetObject()));
      } else if((*wrap)->IsString()) {
        std::string wrap_string((*wrap)->GetString(), (*wrap)->GetStringLength());
        builder.SetWrap(TextureWrap(wrap_string));
      } else {
        LOG(FATAL) << "invalid wrap property type.";
        return nullptr;
      }
    }

    const auto file = doc().GetFileProperty();
    if(file) {
      if((*file)->IsString()) {
        const auto base_path = fmt::format("{0:s}/textures", FLAGS_resources);
        std::string file_path((*file)->GetString(), (*file)->GetStringLength());
        if(!StartsWith(file_path, base_path))
          file_path = fmt::format("{0:s}/{1:s}", base_path, file_path);
        if(!StartsWith(file_path, "file://"))
          file_path = fmt::format("file://{0:s}", file_path);
        const auto image = img::Decode(file_path);
        if(!image) {
          LOG(FATAL) << "failed to decode image from: " << file_path;
          return nullptr;
        }
        builder << image;
      } else {
        LOG(FATAL) << "invalid file property type.";
      }
    }
    return Texture2d::New(builder);
  }
}