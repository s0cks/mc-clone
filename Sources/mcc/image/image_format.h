#ifndef MCC_IMAGE_FORMAT_H
#define MCC_IMAGE_FORMAT_H

#include "mcc/gfx.h"

namespace mcc::img {
  enum ImageFormat : GLenum {
    kRGB      =   GL_RGB,
    kRGBA     =   GL_RGBA,
    kTotalNumberOfImageFormats = 2,
    kDefaultImageFormat = kRGB,
  };
}

#endif //MCC_IMAGE_FORMAT_H