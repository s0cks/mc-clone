#ifndef MCC_IMAGE_CONSTANTS_H
#define MCC_IMAGE_CONSTANTS_H

#include <memory>
#include "mcc/gfx.h"
#include "mcc/glm.h"

namespace mcc::img {
  enum ImageType : GLenum {
    kRGB = GL_RGB,
    kRGBA = GL_RGBA,
  };

  typedef glm::u32vec2 ImageSize;

  class Image;
  typedef std::shared_ptr<Image> ImagePtr;
}

#endif //MCC_IMAGE_CONSTANTS_H