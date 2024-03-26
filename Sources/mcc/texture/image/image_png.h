#ifndef MCC_IMAGE_H
#error "Please #include <mcc/texture/image/image.h> instead."
#endif //MCC_IMAGE_H

#ifndef MCC_PNG_H
#define MCC_PNG_H

#include "mcc/texture/image/image.h"
#include <string>

namespace mcc::img::png  {
  bool Decode(const ImageRef& filename, Image& result);
}

#endif //MCC_PNG_H