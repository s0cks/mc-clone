#ifndef MCC_IMAGE_H
#define MCC_IMAGE_H

#include <string>
#include <functional>
#include <unordered_set>

#include "mcc/rx.h"
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

#include "mcc/file_resolver.h"

namespace mcc::texture {
  static inline bool
  ForEachImageExtension(std::function<bool(const std::string&)> callback) {
    static const std::unordered_set<std::string> kValidImageExtensions = {
      "json",
      "png",
      "jpeg",
      "jpg"
    };
    for(const auto& ext : kValidImageExtensions) {
      if(!callback(ext))
        return false;
    }
    return true;
  }
}

#endif //MCC_IMAGE_H