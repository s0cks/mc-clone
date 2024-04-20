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

  static inline Image*
  Decode(const uri::Uri& uri) {
    const auto file = uri.OpenFileForReading();
    if(!file) {
      DLOG(ERROR) << "failed to open file: " << uri;
      return nullptr;
    }

    const auto image = Decode(file);
    if(!image) {
      DLOG(ERROR) << "failed to decode png from file: " << uri;
      return nullptr;
    }

    const auto error = fclose(file);
    LOG_IF(FATAL, error == EOF) << "failed to close file: " << uri;
    return image;
  }

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