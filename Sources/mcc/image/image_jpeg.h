#ifndef MCC_IMAGE_H
#error "Please #include <mcc/image/image.h> instead."
#endif //MCC_IMAGE_H

#ifndef MCC_JPEG_H
#define MCC_JPEG_H

#include <string>

#include "mcc/image/image_decoder.h"

namespace mcc::img::jpeg {
  bool Decode(const std::string& filename, Image& result);

  class JpegImageDecoder : public ImageDecoder {
  protected:
    ImagePtr DecodeJPEG(FILE* file);
  public:
    explicit JpegImageDecoder(const uri::Uri& uri):
      ImageDecoder(uri) {
    }
    ~JpegImageDecoder() override = default;
    rx::observable<ImagePtr> Decode() override;
  };
}

#endif //MCC_JPEG_H