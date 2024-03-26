#include "mcc/texture/texture.h"
#include <regex>

#include "mcc/uri.h"
#include "mcc/flags.h"
#include "mcc/texture/image/image.h"
#include "mcc/texture/texture_loader.h"

namespace mcc {
  TextureRef GetTexture(const uri::Uri& uri) {
    DLOG(INFO) << "checking for: " << uri;
    auto location = FLAGS_resources + "/textures/" + uri.path;
    return TextureRef();
  }
}