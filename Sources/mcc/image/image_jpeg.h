#ifndef MCC_IMAGE_H
#error "Please #include <mcc/image/image.h> instead."
#endif //MCC_IMAGE_H

#ifndef MCC_JPEG_H
#define MCC_JPEG_H

#include <string>
#include <unordered_set>

#include "mcc/uri.h"
#include "mcc/image/image_decoder.h"

namespace mcc::img::jpeg {
  static const std::unordered_set<std::string> kValidExtensions = {
    "jpg",
    "jpeg",
  };

  static inline bool
  Filter(const uri::Uri& uri) {
    return uri.HasScheme("file")
        && uri.HasExtension(kValidExtensions);
  }

  Image* Decode(FILE* file);
  Image* Decode(const uri::Uri& uri);

  rx::observable<Image*> DecodeAsync(const uri::Uri& uri);

  static inline rx::observable<Image*>
  DecodeAsync(const uri::basic_uri& uri) {
    return DecodeAsync(uri::Uri(uri));
  }
}

#endif //MCC_JPEG_H