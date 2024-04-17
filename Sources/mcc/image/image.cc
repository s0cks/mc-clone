#include "mcc/image/image.h"

namespace mcc::img {
  rx::observable<ImagePtr> GetImage(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("file"));
    const auto ext = uri.GetPathExtension();
    MCC_ASSERT(ext);
    if(EqualsIgnoreCase(*ext, "png")) {
      png::PngImageDecoder decoder(uri);
      return decoder.Decode();
    } else if(EqualsIgnoreCase(*ext, "jpg") || EqualsIgnoreCase(*ext, "jpeg")) {
      jpeg::JpegImageDecoder decoder(uri);
      return decoder.Decode();
    }

    const auto err = fmt::format("invalid extension {0:s} for image: {1:s}", (*ext), (const std::string&) uri);
    return rx::observable<>::error<ImagePtr>(std::runtime_error(err));
  }
}