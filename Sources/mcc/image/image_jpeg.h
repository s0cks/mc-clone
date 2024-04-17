#ifndef MCC_IMAGE_H
#error "Please #include <mcc/image/image.h> instead."
#endif //MCC_IMAGE_H

#ifndef MCC_JPEG_H
#define MCC_JPEG_H

#include <string>

#include "mcc/image/image_decoder.h"

namespace mcc::img::jpeg {
  Image* Decode(FILE* file);
  rx::observable<Image*> DecodeAsync(const uri::Uri& uri);

  static inline rx::observable<Image*>
  DecodeAsync(const uri::basic_uri& uri) {
    return DecodeAsync(uri::Uri(uri));
  }
}

#endif //MCC_JPEG_H