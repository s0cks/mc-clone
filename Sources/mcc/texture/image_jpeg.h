#ifndef MCC_IMAGE_H
#error "Please #include <mcc/texture/image.h> instead."
#endif //MCC_IMAGE_H

#ifndef MCC_JPEG_H
#define MCC_JPEG_H

#include <string>

namespace mcc::jpeg {
  bool Decode(const std::string& filename, Image& result);
}

#endif //MCC_JPEG_H