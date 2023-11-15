#ifndef MCC_IMAGE_H
#define MCC_IMAGE_H

#include <string>
#include "mcc/gfx.h"
#include "mcc/buffer.h"

namespace mcc {
  typedef glm::u64vec2 Dimension;

  struct Image {
    enum Type : GLenum {
      kRGB = GL_RGB,
      kRGBA = GL_RGBA,
    };

    Type type;
    Dimension size;
    BufferPtr data;
  };
}

#include "mcc/texture/image_png.h"
#include "mcc/texture/image_jpeg.h"

#endif //MCC_IMAGE_H