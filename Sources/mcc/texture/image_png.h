#ifndef MCC_IMAGE_H
#error "Please #include <mcc/texture/image.h> instead."
#endif //MCC_IMAGE_H

#ifndef MCC_PNG_H
#define MCC_PNG_H

#include <string>

namespace mcc::png {
  bool Decode(const std::string& filename, Image& result);
}

#endif //MCC_PNG_H