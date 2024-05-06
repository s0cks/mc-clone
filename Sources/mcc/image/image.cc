#include "mcc/image/image.h"

namespace mcc::img {
  const std::unordered_set<std::string>&
  GetValidExtensions() {
    static std::unordered_set<std::string> extensions;
    if(!extensions.empty())
      return extensions;
    extensions.insert(std::begin(png::kValidExtensions), std::end(png::kValidExtensions));
    extensions.insert(std::begin(jpeg::kValidExtensions), std::end(jpeg::kValidExtensions));
    return extensions;
  }

  bool Filter(const uri::Uri& uri) {
    return uri.HasScheme("file")
        && uri.HasExtension(GetValidExtensions());
  }

  Image* Decode(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("file"));
    MCC_ASSERT(uri.HasExtension());
    if(png::Filter(uri)) {
      return png::Decode(uri);
    } else if(jpeg::Filter(uri)) {
      return jpeg::Decode(uri);
    }

    LOG(ERROR) << "invalid image uri: " << uri;
    return nullptr;
  }

  rx::observable<Image*> DecodeAsync(const uri::Uri& uri) {
    return rx::observable<>::create<Image*>([uri](rx::subscriber<Image*> s) {
      const auto image = Decode(uri);
      if(!image) {
        const auto err = fmt::format("failed to decode image: {0:s}", (const std::string&) uri);
        return s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
      }
      s.on_next(image);
      s.on_completed();
    });
  }
}