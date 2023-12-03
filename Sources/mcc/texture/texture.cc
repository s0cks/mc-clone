#include "mcc/texture/texture.h"
#include <regex>

#include "mcc/uri.h"
#include "mcc/flags.h"
#include "mcc/texture/image.h"
#include "mcc/texture/texture_loader.h"
#include "mcc/texture/texture_resolver.h"

namespace mcc {
  TextureRef GetTexture(const uri::Uri& uri) {
    DLOG(INFO) << "checking for: " << uri;
    auto location = FLAGS_resources + "/textures/" + uri.path;
    return TextureRef();
  }
}