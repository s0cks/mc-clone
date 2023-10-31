#include "mcc/texture/texture.h"
#include <regex>

#include "mcc/texture/texture_loader.h"

namespace mcc::texture {
  static const std::regex kPngPattern("^.*\\.png$");

  Texture Texture::LoadFrom(const std::string& filename) {
    if(std::regex_match(filename, kPngPattern)) {
      PngTextureLoader loader(filename);
      return loader.Load();
    }

    return Texture(kInvalidTextureId);
  }
}