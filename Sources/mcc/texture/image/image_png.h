#ifndef MCC_IMAGE_H
#error "Please #include <mcc/texture/image/image.h> instead."
#endif //MCC_IMAGE_H

#ifndef MCC_PNG_H
#define MCC_PNG_H

#include "mcc/texture/image/image.h"
#include <string>
#include <optional>
#include "mcc/texture/image/image_decoder.h"

namespace mcc::img::png {
  class PngImageDecoder : public ImageDecoder {
  protected:
    ImagePtr DecodePNG(FILE* file);
  public:
    explicit PngImageDecoder(const uri::Uri& uri):
      ImageDecoder(uri) {
    }
    ~PngImageDecoder() override = default;
    rx::observable<ImagePtr> Decode() override;
  };
}

#endif //MCC_PNG_H