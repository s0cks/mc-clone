#ifndef MCC_IMAGE_H
#define MCC_IMAGE_H

#include <string>
#include "mcc/glm.h"
#include "mcc/buffer.h"

namespace mcc::img {
  typedef std::string ImageRef;
  typedef glm::u64vec2 Dimension;

  struct Image {
    DEFINE_NON_COPYABLE_TYPE(Image);
    enum Type : uint8_t {
      kRGB,
      kRGBA
    };

    enum State : uint8_t {
      kUninitialized,
      kInitialized,
      kError,
      kNotFound,
    };

    struct Dimension {
      uint64_t width;
      uint64_t height;
    };

    std::string filename;
    State state;
    Type type;
    img::Dimension size;

    uint8_t* bytes;
    uint64_t num_bytes;

    Image() = default;
    Image(const ImageRef& src);
    ~Image() = default;

    friend std::ostream& operator<<(std::ostream& stream, const Image& rhs) {
      stream << "Image(";
      stream << "filename=" << rhs.filename << ", ";
      stream << "state=" << rhs.state << ", ";
      stream << "type=" << rhs.type << ", ";
      stream << "size=" << glm::to_string(rhs.size) << ", ";
      stream << "bytes=" << rhs.num_bytes;
      stream << ")";
      return stream;
    }
  };
}

#include "mcc/texture/image/image_png.h"
#include "mcc/texture/image/image_jpeg.h"

#endif //MCC_IMAGE_H