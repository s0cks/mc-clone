#include "mcc/image/image.h"

namespace mcc::img {
  rx::observable<Image*> GetImage(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("file"));
    const auto ext = uri.GetPathExtension();
    MCC_ASSERT(ext);
    if(EqualsIgnoreCase(*ext, "png")) {
      return png::DecodeAsync(uri);
    } else if(EqualsIgnoreCase(*ext, "jpg") || EqualsIgnoreCase(*ext, "jpeg")) {
      return jpeg::DecodeAsync(uri);
    }

    const auto err = fmt::format("invalid extension {0:s} for image: {1:s}", (*ext), (const std::string&) uri);
    return rx::observable<>::error<Image*>(std::runtime_error(err));
  }
}