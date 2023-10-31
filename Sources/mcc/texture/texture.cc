#include "mcc/texture/texture.h"
#include <regex>

#include "mcc/flags.h"
#include "mcc/texture/texture_loader.h"

namespace mcc::texture {
  static const std::regex kPngPattern(".*\\.png$");

  static inline bool
  BeginsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() 
        && str.compare(0, prefix.size(), prefix) == 0;
  }

  Texture Texture::LoadFrom(std::string filename) {
    if(!BeginsWith(filename, FLAGS_resources + "/textures/"))
      filename = FLAGS_resources + "/textures/";

    if(std::regex_match(filename, kPngPattern)) {
      PngTextureLoader loader(filename);
      return loader.Load();
    }

    return Texture(kInvalidTextureId);
  }
}