#ifndef MCC_IMAGE_H
#error "Please #include <mcc/texture/image/image.h> instead."
#endif //MCC_IMAGE_H

#ifndef MCC_PNG_H
#define MCC_PNG_H

#include <string>
#include <optional>
#include <unordered_set>

#include "mcc/image/image.h"
#include "mcc/image/image_decoder.h"

namespace mcc::img::png {
  static const std::unordered_set<std::string> kValidExtensions = {
    "png",
  };

  static inline bool
  Filter(const uri::Uri& uri) {
    return uri.HasScheme("file")
        && uri.HasExtension(kValidExtensions);
  }

  Image* Decode(FILE* file);
  Image* Decode(const uri::Uri& uri);

  static inline Image*
  Decode(const uri::basic_uri& uri) {
    return Decode(uri::Uri(uri));
  }

  rx::observable<Image*> DecodeAsync(const uri::Uri& uri);

  static inline rx::observable<Image*>
  DecodeAsync(const uri::basic_uri& uri) {
    return DecodeAsync(uri::Uri(uri));
  }
}

#endif //MCC_PNG_H