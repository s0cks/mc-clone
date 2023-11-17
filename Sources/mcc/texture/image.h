#ifndef MCC_IMAGE_H
#define MCC_IMAGE_H

#include <string>
#include "mcc/gfx.h"
#include "mcc/buffer.h"

namespace mcc {
  struct Image {
    enum Type : GLenum {
      kRGB = GL_RGB,
      kRGBA = GL_RGBA,
    };

    friend std::ostream& operator<<(std::ostream& stream, const Type& rhs) {
      switch(rhs) {
        case kRGBA:
          return stream << "RGBA";
        case kRGB:
          return stream << "RGB";
        default:
          return stream << "<unknown Image::Type: " << static_cast<uint32_t>(rhs) << ">";
      }
    }

    Type type;
    Dimension size;
    BufferPtr data;
  };
}

#include "mcc/texture/image_png.h"
#include "mcc/texture/image_jpeg.h"

#endif //MCC_IMAGE_H