#include "mcc/texture/image/image.h"

namespace mcc::img {
  Image::Image(const ImageRef& src):
    filename(src),
    state(Image::kUninitialized),
    type(Image::kRGBA),
    size(0) {
  }
}