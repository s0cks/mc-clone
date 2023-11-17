#include "mcc/texture/texture.h"
#include <regex>

#include "mcc/flags.h"
#include "mcc/texture/image.h"
#include "mcc/texture/texture_loader.h"

namespace mcc {
  static const std::regex kPngPattern(".*\\.(png)$");
  static const std::regex kJpegPattern(".*\\.(jpeg|jpg)$");
  static const std::regex kJsonPattern(".*\\.(json)$");

  TextureRef GetTexture(const resource::Token& token) {
    const auto& tag = token.tag;
    MCC_ASSERT(tag.type() == resource::kTextureType);
    const auto& filename = token.location;
    texture::TextureLoader loader(tag, filename);
    return loader.Load();
  }
}