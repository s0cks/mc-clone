#ifndef MCC_IMAGE_H
#error "Please #include <mcc/texture/image/image.h> instead."
#endif //MCC_IMAGE_H

#ifndef MCC_PNG_H
#define MCC_PNG_H

#include <string>
#include <optional>

#include "mcc/image/image.h"
#include "mcc/image/image_decoder.h"

namespace mcc::img::png {
  Image* Decode(FILE* file);
  rx::observable<Image*> DecodeAsync(const uri::Uri& uri);

  static inline rx::observable<Image*>
  DecodeAsync(const uri::basic_uri& uri) {
    return DecodeAsync(uri::Uri(uri));
  }
}

#endif //MCC_PNG_H