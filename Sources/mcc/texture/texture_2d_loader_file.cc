#include "mcc/texture/texture_2d_loader_file.h"

namespace mcc::texture {
  Texture2d* Texture2dFileLoader::LoadTexture() const {
    MCC_ASSERT(img::Filter(uri_));
    TextureBuilder builder(k2D);
    LOG_IF(WARNING, !FileExists(uri_)) << "cannot load Texture2d from " << uri_ << ", file doesn't exist.";
    if(FileExists(uri_)) {
      const auto image = img::Decode(uri_);
      builder << image;
    }
    return Texture2d::New(builder);
  }
}